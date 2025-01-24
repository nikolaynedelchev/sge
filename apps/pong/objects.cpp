#include "objects.h"
#include "components.h"

namespace ndn::pong::objects
{

Entity CreatePlayer()
{
    auto entity = World().create();
    World().emplace<comp::Object2d>(entity);
    World().emplace<comp::Sprite>(entity);
    World().emplace<comp::Collision2d>(entity);
    World().emplace<comp::Player>(entity);

    //
    auto& obj = World().get<comp::Object2d>(entity);
    obj.position = {300, 500};
    obj.rotation = 0.0f;
    obj.scale = {1, 1};
    obj.visible = true;

    auto& sprite = World().get<comp::Sprite>(entity);
    sprite.texture = 0;
    sprite.src = {0, 0, 160, 40};
    sprite.SetAnchor(Anchors::centered);

    auto& collision = World().get<comp::Collision2d>(entity);
    collision.mainRect = {0, 0, sprite.src.width, sprite.src.height};

    obj.position += sprite.anchor;

}

}
