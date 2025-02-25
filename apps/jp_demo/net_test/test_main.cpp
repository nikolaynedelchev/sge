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
int TestDummyServer();

Ptr<std::thread> RunServerThread();
void StopServer();
Ptr<std::thread> RunClientThread();

int Main()
{
    //return TestDummyServer();
    fmt::println("Initing server...");
    auto serverThread = RunServerThread();
    std::vector<Ptr<std::thread>> clients;
    for(int i = 0; i < 1; i++)
    {
        clients.push_back( RunClientThread() );
    }

    tools::Stopwatch sw;

    for(auto& t : clients)
    {
        t->join();
    }

    StopServer();
    serverThread->join();
    fmt::println("Time: {}", sw.Measure());

    return 0;
}
}
