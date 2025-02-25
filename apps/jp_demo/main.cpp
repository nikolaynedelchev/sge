#include <tools/tools.h>
#include <fmt/format.h>
#include <memory.h>
#include <tools/Stopwatch.h>
#include <tools/Rng.h>
#include <filesystem>
#include <thread>
#include <uvw.hpp>
#include <chrono>

namespace ndn::jp_demo{int Main();}
int main(){return ::ndn::jp_demo::Main();}

namespace ndn::jp_demo
{

int Main()
{
    fmt::println("Initing server...");
    auto loop = uvw::loop::get_default();
    auto server = loop->resource<uvw::tcp_handle>();
    server->bind("0.0.0.0", 8080);
    server->listen();

    auto timer = loop->resource<uvw::timer_handle>();
    timer->on<uvw::timer_event>([&server, &loop](const uvw::timer_event &, uvw::timer_handle &timer) {
        std::cout << "Shutting down server...\n";
        server->close();
        loop->stop();
    });

    fmt::println("Start timer...");
    timer->start(uvw::timer_handle::time{3000}, uvw::timer_handle::time{0});

    fmt::println("Start loop...");
    loop->run();
    fmt::println("Loop End");

    fmt::println("Destroing timer...");
    timer.reset();

    fmt::println("Destroing server...");
    server.reset();

    fmt::println("Destroing loop...");
    loop.reset();


    fmt::println("Good bye");
    return 0;
}

}
