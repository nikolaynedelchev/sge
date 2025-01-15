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
            using TypePrecision = Precision;
            using TypePoint = Point<TypePrecision, 2>;
            using TypeVec = Vec<TypePrecision, 2>;

            TypePoint pos;
            TypeVec size;
        };

        template<typename Precision>
        struct Circle
        {
            using TypePrecision = Precision;
            using TypePoint = Point<TypePrecision, 2>;

            TypePoint pos;
            TypePrecision r;
        };

        template<typename Precision>
        struct Segment
        {
            using TypePrecision = Precision;
            using TypePoint = Point<TypePrecision, 2>;

            TypePoint p1;
            TypePoint p2;
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
