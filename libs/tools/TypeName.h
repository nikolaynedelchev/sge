#pragma once

#include <string>
#include <typeinfo>

namespace ndn::tools
{

template<typename T>
std::string TypeName()
{
    return typeid(T).name();
}

}
