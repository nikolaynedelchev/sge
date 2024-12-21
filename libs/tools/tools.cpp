#include "tools.h"
#include "iostream"
#include <fmt/printf.h>

namespace ndn::tools
{
void test_me()
{
    fmt::println("Format lib tested from tools lib: {}", 666);
    std::cout << "Tools lib tesed" << std::endl;
}
}
