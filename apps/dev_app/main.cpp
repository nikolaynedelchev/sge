#include "../../libs/tools/tools.h"
#include <raylib.h>
#include <tools/tools.h>
#include <fmt/format.h>
#include <entt/entt.hpp>
#include <sge/sge.h>
#include <glm/glm.hpp>
#include <glm/geometric.hpp>

int main()
{

    glm::vec2 v1 = {0.5, 1.4};
    glm::vec2 v2 = {12.5, 1.6};
    auto dist = glm::distance(v1, v2);
    ndn::sge::engine eng;
    eng.init("./", 320, 200);
    ndn::tools::test_me();
    ndn::tools::test_me2();
    entt::registry registry;
    auto entity = registry.create();

    struct test_comp{int a;};
    registry.emplace<test_comp>(entity);
    (void)registry;


    fmt::println("Closing...");

    eng.close();
    fmt::println("Goodbye...");

    return 0;
}
