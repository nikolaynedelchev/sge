#include "operators.h"

using namespace ndn::sge;

///////////////////////
// Points
ndn::sge::point& operator+=(ndn::sge::point& v1, const ndn::sge::point& v2)
{
    v1.x += v2.x;
    v1.y += v2.y;
    return v1;
}

ndn::sge::point operator+(const ndn::sge::point& v1, const ndn::sge::point& v2)
{
    auto r = v1;
    r.x += v2.x;
    r.y += v2.y;
    return r;
}

ndn::sge::point& operator-=(ndn::sge::point& v1, const ndn::sge::point& v2)
{
    v1.x -= v2.x;
    v1.y -= v2.y;
    return v1;
}

ndn::sge::point operator-(const ndn::sge::point& v1, const ndn::sge::point& v2)
{
    auto r = v1;
    r.x -= v2.x;
    r.y -= v2.y;
    return r;
}

ndn::sge::point& operator*=(ndn::sge::point& v1, const ndn::sge::point& v2)
{
    v1.x *= v2.x;
    v1.y *= v2.y;
    return v1;
}

ndn::sge::point operator*(const ndn::sge::point& v1, const ndn::sge::point& v2)
{
    auto r = v1;
    r.x *= v2.x;
    r.y *= v2.y;
    return r;
}

ndn::sge::point& operator/=(ndn::sge::point& v1, const ndn::sge::point& v2)
{
    v1.x /= v2.x;
    v1.y /= v2.y;
    return v1;
}

ndn::sge::point operator/(const ndn::sge::point& v1, const ndn::sge::point& v2)
{
    auto r = v1;
    r.x /= v2.x;
    r.y /= v2.y;
    return r;
}

ndn::sge::point& operator+=(ndn::sge::point& v1, float v2)
{
    v1.x += v2;
    v1.y += v2;
    return v1;
}

ndn::sge::point operator+(const ndn::sge::point& v1, float v2)
{
    auto r = v1;
    r.x += v2;
    r.y += v2;
    return r;
}

ndn::sge::point& operator-=(ndn::sge::point& v1, float v2)
{
    v1.x -= v2;
    v1.y -= v2;
    return v1;
}

ndn::sge::point operator-(const ndn::sge::point& v1, float v2)
{
    auto r = v1;
    r.x -= v2;
    r.y -= v2;
    return r;
}

ndn::sge::point& operator*=(ndn::sge::point& v1, float v2)
{
    v1.x *= v2;
    v1.y *= v2;
    return v1;
}

ndn::sge::point operator*(const ndn::sge::point& v1, float v2)
{
    auto r = v1;
    r.x *= v2;
    r.y *= v2;
    return r;
}

ndn::sge::point& operator/=(ndn::sge::point& v1, float v2)
{
    v1.x /= v2;
    v1.y /= v2;
    return v1;
}

ndn::sge::point operator/(const ndn::sge::point& v1, float v2)
{
    auto r = v1;
    r.x /= v2;
    r.y /= v2;
    return r;
}

ndn::sge::point& operator+=(float v1, ndn::sge::point& v2)
{
    v2.x += v1;
    v2.y += v1;
    return v2;
}

ndn::sge::point operator+(float v1, ndn::sge::point& v2)
{
    auto r = v2;
    r.x += v1;
    r.y += v1;
    return r;
}

ndn::sge::point& operator-=(float v1, ndn::sge::point& v2)
{
    v2.x -= v1;
    v2.y -= v1;
    return v2;
}

ndn::sge::point operator-(float v1, ndn::sge::point& v2)
{
    auto r = v2;
    r.x -= v1;
    r.y -= v1;
    return r;
}

ndn::sge::point& operator*=(float v1, ndn::sge::point& v2)
{
    v2.x *= v1;
    v2.y *= v1;
    return v2;
}

ndn::sge::point operator*(float v1, ndn::sge::point& v2)
{
    auto r = v2;
    r.x *= v1;
    r.y *= v1;
    return r;
}

ndn::sge::point& operator/=(float v1, ndn::sge::point& v2)
{
    v2.x /= v1;
    v2.y /= v1;
    return v2;
}

ndn::sge::point operator/(float v1, ndn::sge::point& v2)
{
    auto r = v2;
    r.x /= v1;
    r.y /= v1;
    return r;
}

bool operator==(const ndn::sge::point& v1, const ndn::sge::point v2)
{
    return v1.x == v2.x && v1.y == v2.y;
}

///////////////////////
// Rects
ndn::sge::rect& operator+=(ndn::sge::rect& r1, const ndn::sge::point& v2)
{
    r1.x += v2.x;
    r1.y += v2.y;
    return r1;
}

ndn::sge::rect operator+(const ndn::sge::rect& r1, const ndn::sge::point& v2)
{
    auto r = r1;
    r.x += v2.x;
    r.y += v2.y;
    return r;
}

ndn::sge::rect& operator-=(ndn::sge::rect& r1, const ndn::sge::point& v2)
{
    r1.x -= v2.x;
    r1.y -= v2.y;
    return r1;
}

ndn::sge::rect operator-(const ndn::sge::rect& r1, const ndn::sge::point& v2)
{
    auto r = r1;
    r.x -= v2.x;
    r.y -= v2.y;
    return r;
}

///////////////////////
// Circles
// circle& operator+=(circle& c1, const point& v2)
// {
//     c1.center += v2;
//     return c1;
// }
// circle operator+(const circle& c1, const point& v2)
// {
//     auto res = c1;
//     res.center += v2;
//     return res;
// }
// circle& operator-=(circle& c1, const point& v2)
// {
//     c1.center -= v2;
//     return c1;
// }
// circle operator-(const circle& c1, const point& v2)
// {
//     auto res = c1;
//     res.center -= v2;
//     return res;
// }
// circle& operator*=(circle& c1, const float& v2)
// {
//     c1.radius *= v2;
//     return c1;
// }
// circle operator*(const circle& c1, const float& v2)
// {
//     auto res = c1;
//     res.radius *= v2;
//     return res;
// }
// circle& operator/=(circle& c1, const float& v2)
// {
//     c1.radius /= v2;
//     return c1;
// }
// circle operator/(const circle& c1, const float& v2)
// {
//     auto res = c1;
//     res.radius /= v2;
//     return res;
// }
