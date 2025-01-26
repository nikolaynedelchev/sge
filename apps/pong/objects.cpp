#include "objects.h"
#include "components.h"
#include <raymath.h>

namespace ndn::pong::objects
{

Entity CreatePlayer()
{
    auto entity = World().create();

    auto& obj = World().get_or_emplace<comp::Object2d>(entity);
    auto& sprite = World().emplace<comp::Sprite>(entity);
    auto& collision = World().emplace<comp::Collision2d>(entity);
    World().emplace<comp::Player>(entity);

    // object
    obj.position = {300, 500};
    obj.rotation = 0.0f;
    obj.scale = {1, 1};
    obj.visible = true;

    // sprite
    sprite.texture = 0;
    sprite.src = {0, 0, 160, 40};
    sprite.SetAnchor(Anchors::centered);

    // collision
    collision.mainRect = {0, 0, sprite.src.width, sprite.src.height};

    return entity;
}

}
