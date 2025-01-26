#include "geometry.h"

namespace ndn::sge
{

void test_fun_geom()
{
    Point2i p;
    p.x;

    Vec2 v2;

    Rect2f r = {{10, 10}, {5, 4}};
    r.pos = {10, 10};
    r.size = {5, 4};

    Circle2d c;

    //rect.a
}

glm::mat2 RotationMatrix2(double radians)
{
    double s = glm::sin(radians);
    double c = glm::cos(radians);
    return glm::mat2(c, -s,
                     s,  c);
}

}
