#pragma once
#include <glm/glm.hpp>
#include <glm/geometric.hpp>

namespace ndn::sge
{
    using DefaultPrecision = double;

    namespace Generic
    {
        template<typename Precision, int Dimensions>
        using Vec = glm::vec<Dimensions, Precision, glm::defaultp>;

        template<typename Precision, int Dimensions>
        using Point = Vec<Precision, Dimensions>;

        template<typename Precision>
        struct Line
        {

        };

        template<typename Precision>
        struct Rect
        {
            using RectPoint = Point<Precision, 2>;
            using RectVec = Vec<Precision, 2>;
            RectPoint pos;
            RectVec size;

            RectPoint UpLeft() const      {return pos;};
            RectPoint UpRight() const     {return pos + RectVec{size.x, 0};};
            RectPoint DnLeft() const      {return pos + RectVec{0, size.y};};
            RectPoint DnRight() const     {return pos + size.x;};
            RectPoint UpMid() const       {return pos + RectVec{size.x / 2, 0};};
            RectPoint DnMid() const       {return pos + RectVec{size.x, size.y};};
            RectPoint LeftMid() const     {return pos + RectVec{0, size.y / 2};};
            RectPoint RightMid() const    {return pos + RectVec{0, size.y / 2};};
            RectPoint Center() const      {return pos + RectVec{size.x / 2, size.y / 2};};
        };

        template<typename Precision>
        struct Circle
        {
            using CirclePoint = Point<Precision, 2>;
            using CircleVec = Vec<Precision, 2>;
            using CircleRect = Rect<Precision>;

            CirclePoint pos;
            Precision r;

            CirclePoint Center() const  {return pos;};
            CirclePoint Up() const      {return pos - CircleVec{0, r};};
            CirclePoint Dn() const      {return pos + CircleVec{0, r};};
            CirclePoint Left() const    {return pos - CircleVec{r, 0};};
            CirclePoint Right() const   {return pos + CircleVec{0, r};};
            CircleRect Rectangle() const {return CircleRect{CircleRect::RectPoint(pos - CircleVec{r, r}), CircleRect::RectVec(2 * r, 2 * r)};};
        };

    }

    using Vec2i = Generic::Vec<int, 2>;
    using Vec2f = Generic::Vec<float, 2>;
    using Vec2d = Generic::Vec<double, 2>;
    using Vec2 = Generic::Vec<DefaultPrecision, 2>;

    using Point2i = Vec2i;
    using Point2f = Vec2f;
    using Point2d = Vec2d;
    using Point2 = Vec2;

    using Rect2i = Generic::Rect<int>;
    using Rect2f = Generic::Rect<float>;
    using Rect2d = Generic::Rect<double>;
    using Rect2 = Generic::Rect<DefaultPrecision>;

    using Circle2i = Generic::Rect<int>;
    using Circle2f = Generic::Rect<float>;
    using Circle2d = Generic::Rect<double>;
    using Circle2 = Generic::Circle<DefaultPrecision>;
}
