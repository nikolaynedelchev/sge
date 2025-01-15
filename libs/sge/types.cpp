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

    Ray2f r1 = {Point2f(0.0f, 5.0f), Point2f{3.0f, 6.0f}};
    auto r2 = r1.RayBA();

    //rect.a
}
}
