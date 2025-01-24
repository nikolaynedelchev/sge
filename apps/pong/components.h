#pragma once
#include <raylib.h>
#include "types.h"
#include <vector>
#include <cstdint>
#include <any>
#include <map>
#include <string>

namespace ndn::pong::comp
{
    struct Object2d
    {
        Vector2 position;
        Vector2 scale;
        float rotation;
        bool visible;
    };

    struct Sprite
    {
        uint64_t texture;
        Rectangle src;
        Vector2 anchor;

        void SetAnchor(Anchors a);
    };

    struct Collision2d
    {
        Rectangle mainRect;
        std::vector<Rectangle> rects;
        std::vector<Circle> circles;
        std::vector<Entity> collisions;
    };

    struct Player
    {

    };

    struct Ball
    {

    };
}
