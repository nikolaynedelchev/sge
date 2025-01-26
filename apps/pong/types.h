#pragma once
#include <raylib.h>
#include <entt/entt.hpp>
#include <cstdint>
#include <tools/Runtime.h>

namespace ndn::pong
{

std::unique_ptr<tools::Runtime> SwitchRuntimeContext(std::unique_ptr<tools::Runtime> runtime);
tools::Runtime& Runtime();

using WorldRegistry = entt::registry;
using Entity = entt::entity;
WorldRegistry& World();


enum class Anchors
{
    up_left,
    up_mid,
    up_right,
    mid_left,
    centered,
    mid_right,
    down_left,
    down_mid,
    down_right
};
Vector2 CalculateAnchor(Anchors a, float width, float height);

struct Circle
{
    Vector2 position;
    float radius;
};




}
