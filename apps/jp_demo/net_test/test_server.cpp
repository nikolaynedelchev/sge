#include <tools/tools.h>
#include <fmt/format.h>
#include <memory.h>
#include <tools/Stopwatch.h>
#include <tools/Rng.h>
#include <filesystem>
#include <thread>
#include <uvw.hpp>
#include <chrono>
#include <tools/tools.h>

namespace ndn::jp_demo::net_test
{

struct ClientCtx
{
    std::vector<std::string> msgs;
    std::string notReady;
};

class Server
{
public:

    void Stop()
    {
        m_async->send();
    }

    void Run()
    {
        m_loop = uvw::loop::create();
        m_server = m_loop->resource<uvw::tcp_handle>();
        m_server->on<uvw::listen_event>([this](const uvw::listen_event& listenEvent, uvw::tcp_handle & client) {
            OnNewConnection(listenEvent, client);
        });
        m_server->bind("0.0.0.0", 12345);
        m_server->listen();

        m_async = m_loop->resource<uvw::async_handle>();
        m_async->on<uvw::async_event>([this](const uvw::async_event& ev, uvw::async_handle& handle){
            m_loop->walk([](auto& handle){
                handle.close();
            });
            m_loop->stop();
        });
        m_loop->run();
        fmt::println("Server closed");
    }

    void OnNewConnection(const uvw::listen_event& listenEvent, uvw::tcp_handle & client)
    {
        m_server->accept(client);
        std::cout << "Client connected\n";

        client.on<uvw::close_event>([](const uvw::close_event & closeEvent, uvw::tcp_handle & client) {
            //std::cout << "Client disconnected\n";
        });

        client.on<uvw::data_event>([this, ctx = ClientCtx()](const uvw::data_event &event, uvw::tcp_handle &client) mutable {
            OnDataEvent(event, client, ctx);
        });

        client.read();
    }

    void OnDataEvent(const uvw::data_event &event, uvw::tcp_handle &client, ClientCtx& ctx)
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
        ProcessMsgs(client, ctx);
    }

    void ProcessMsgs(uvw::tcp_handle &client, ClientCtx& ctx)
    {
        for(const auto& msg : ctx.msgs)
        {
            auto data = UPtr<char[]>(new char[msg.size()]);
            std::memcpy(data.get(), msg.data(), msg.size());
            client.write(std::move(data), msg.size()); // Echo back
        }
        ctx.msgs.clear();
    }
    Ptr<uvw::loop> m_loop;
    Ptr<uvw::tcp_handle> m_server;
    Ptr<uvw::async_handle> m_async;
};

static Server* s = nullptr;

void StopServer()
{
    if (s)
    {
        s->Stop();
    }
}

Ptr<std::thread> RunServerThread()
{
    Ptr<std::thread> t = Ptr<std::thread>(new std::thread([]() mutable{
        Server srv;
        s = &srv;
        srv.Run();
    }));
    tools::Stopwatch::SleepForMs(10);
    return t;
}
}
