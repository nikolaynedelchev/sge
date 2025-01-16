#include "types.h"

namespace ndn::sge
{

void test_fun()
{
    Point2i p = {5, 6};
    p.x;

    Vec2 v2;

    Rect2f r = {{10, 10}, {5, 4}};
    r.pos = {10, 10};
    r.size = {5, 4};

    Circle2d c;

    //rect.a
}
}
