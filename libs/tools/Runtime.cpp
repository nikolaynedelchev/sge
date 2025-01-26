#include "Runtime.h"

namespace ndn::tools
{

void Runtime::RemoveAll() noexcept
{
    m_objects.clear();
}

}
