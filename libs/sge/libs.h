#pragma once

#include <iostream>
#include <string>
//#include <vector>
//#include <map>
//#include <cinttypes>
//#include <cmath>
#include <fmt/format.h>

namespace ndn::sge
{
    
template <typename S, typename... T>
inline void print(const S& fmt, T&&... args)
{
    std::cout << vformat(fmt, fmt::make_format_args(args...));
}

template <typename S, typename... T>
inline void println(const S& fmt, T&&... args)
{
    std::cout << vformat(fmt, fmt::make_format_args(args...)) << std::endl;
}


void println();

template <typename S, typename... T>
[[noreturn]]
inline void fatal_error(const S& fmt, T&&... args)
{
    std::string msg = vformat(fmt, fmt::make_format_args(args...));
    std::cout << msg << std::endl << std::flush;
    throw std::runtime_error(msg);
}

}
//#include "operators.h"
