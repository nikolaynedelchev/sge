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

int TestDummyServer()
{
    fmt::println("Initing server...");
    auto loop = uvw::loop::create();
    auto server = loop->resource<uvw::tcp_handle>();
    server->on<uvw::listen_event>([](const uvw::listen_event& e, uvw::tcp_handle& handle){

    });
    server->on<uvw::close_event>([&](const uvw::close_event& e, uvw::tcp_handle& handle){
        fmt::println("Server closed");
        loop->walk([](auto& handle){
            handle.close();
        });
        loop->stop();
    });
    server->bind("0.0.0.0", 12345);
    server->listen();

    auto timer = loop->resource<uvw::async_handle>();
    timer->on<uvw::async_event>([&](const uvw::async_event& e, uvw::async_handle& handle){
        fmt::println("Server loop stopping...");
        timer->close();
        server->close();
    });

    auto t = std::thread([&](){
        fmt::println("Timer started");
        tools::Stopwatch::SleepForMs(3000);
        fmt::println("Timer fired, stopping server...");
        timer->send();
    });

    fmt::println("Server loop started");
    loop->run();
    fmt::println("Server killed");
    t.join();
    return 0;
}
}
