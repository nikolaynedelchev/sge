#include <tools/tools.h>
#include <fmt/format.h>
#include <memory.h>
#include <tools/Stopwatch.h>
#include <tools/Rng.h>
#include <tools/tools.h>
#include <filesystem>
#include <thread>
#include <uvw.hpp>
#include <chrono>

namespace ndn::jp_demo::net_test
{

#define MSG_COUNT (100)

struct ServerCtx
{
    std::vector<std::string> msgs;
    std::string notReady;
};

class Client
{
public:

    void Run()
    {
        m_loop = uvw::loop::create();
        m_client = m_loop->resource<uvw::tcp_handle>();
        m_client->on<uvw::connect_event>([this](const uvw::connect_event& connectEvent, uvw::tcp_handle & client) {
            OnNewConnection(connectEvent, client);
        });

        m_client->on<uvw::close_event>([this](const uvw::close_event& e, uvw::tcp_handle& handle){
            m_loop->walk([](auto& handle){
                handle.close();
            });
            m_loop->stop();
        });

        m_async = m_loop->resource<uvw::async_handle>();
        m_async->on<uvw::async_event>([this, n = 'a'](const uvw::async_event& event, uvw::async_handle& handle) mutable{
            if (true == m_shoudClose)
            {
                m_connected = false;
                m_async->close();
                m_client->close();
                return;
            }
            if (!m_connected)
            {
                return;
            }
            auto data = UPtr<char[]>(new char[8]);
            for(size_t i = 0; i < 8; i++)
            {
                n++;
                data[i] = n;
            }
            m_client->write(std::move(data), 8);
        });

        m_client->connect("127.0.0.1", 12345);

        auto timer = std::thread([&](){
            tools::Stopwatch::SleepForMs(500);
            for(int i = 0; i < MSG_COUNT; i++)
            {
                if (i % 10 == 0)
                {
                    tools::Stopwatch::SleepForMs(1);
                }
                m_async->send();
                if (i % 300 == 0)
                {
                    fmt::println("{} / {}", i, MSG_COUNT);
                }
            }

            while(m_replies != MSG_COUNT)
            {
                tools::Stopwatch::SleepForMs(100);
                fmt::println("wait: {}", m_replies.load() );
            }
            m_shoudClose.store(true);
            m_async->send();
        });
        m_loop->run();
        timer.join();
        fmt::println("Client closed");
    }

    void OnNewConnection(const uvw::connect_event& connectEvent, uvw::tcp_handle & handle)
    {
        handle.on<uvw::data_event>([this, ctx = ServerCtx()](const uvw::data_event &event, uvw::tcp_handle &server) mutable {
            OnDataEvent(event, server, ctx);
        });

        handle.read();
        m_connected = true;
    }

    void OnDataEvent(const uvw::data_event &event, uvw::tcp_handle &server, ServerCtx& ctx)
    {
        for (size_t i = 0; i < event.length; i++)
        {
            ctx.notReady.push_back(event.data[i]);
            if (ctx.notReady.size() == 8)
            {
                ctx.msgs.push_back(std::move(ctx.notReady));
                ctx.notReady = {};
                ctx.notReady.reserve(8);
            }
        }
        ProcessMsgs(server, ctx);
    }

    void ProcessMsgs(uvw::tcp_handle &server, ServerCtx& ctx)
    {
        static size_t t = 0;
        for(const auto& msg : ctx.msgs)
        {
            t += msg.size();
            m_replies.fetch_add(1);
        }
        ctx.msgs.clear();
    }
    bool m_connected = false;
    Ptr<uvw::loop> m_loop;
    Ptr<uvw::tcp_handle> m_client;
    Ptr<uvw::async_handle> m_async;
    std::atomic_bool m_shoudClose = false;
    std::atomic_int m_replies = 0;
};


Ptr<std::thread> RunClientThread()
{
    Ptr<std::thread> t = Ptr<std::thread>(new std::thread([]() mutable{
        Client cln;
        cln.Run();
        fmt::println("Thread closing");
    }));
    tools::Stopwatch::SleepForMs(10);
    return t;
}
}
