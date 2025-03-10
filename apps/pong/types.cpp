#include "types.h"

namespace ndn::pong
{

static std::unique_ptr<tools::Runtime> s_runtime = nullptr;
std::unique_ptr<tools::Runtime> SwitchRuntimeContext(std::unique_ptr<tools::Runtime> runtime)
{
    auto tmp = std::move(s_runtime);
    s_runtime = std::move(runtime);
    return tmp;
}

tools::Runtime &Runtime()
{
    return *s_runtime;
}

WorldRegistry& World()
{
    static WorldRegistry s_registry;
    return s_registry;
}

Vector2 CalculateAnchor(Anchors a, float width, float height)
{
    switch (a) {
    case Anchors::up_left:      return Vector2{0, 0};
    case Anchors::up_mid:       return Vector2{width / 2.0f, 0.0f};
    case Anchors::up_right:     return Vector2{width, 0.0f};
    case Anchors::mid_left:     return Vector2{0.0f, height / 2.0f};
    case Anchors::centered:     return Vector2{width / 2.0f, height / 2.0f};
    case Anchors::mid_right:    return Vector2{width, height / 2.0f};
    case Anchors::down_left:    return Vector2{0.0f, height};
    case Anchors::down_mid:     return Vector2{width / 2.0f, height};
    case Anchors::down_right:   return Vector2{width, height};
    default:
        return Vector2{0, 0};
    }
}

}
