#pragma once
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

namespace ndn::sge
{
    using DefaultPrecision = double;

    namespace Generic
    {
        template<typename Precision, int Dimensions>
        using Vec = glm::vec<Dimensions, Precision, glm::defaultp>;

        template<typename Precision, int Dimensions>
        using Point = Vec<Precision, Dimensions>;

        template<typename Pb, typename Pa, typename Pc>
        double AngleRad(const Point<Pb, 2>& b, const Point<Pa, 2>& a, const Point<Pc, 2>& c)
        {
            return glm::angle(glm::normalize(b - a), glm::normalize(c - a));
        }

        template<typename Pb, typename Pa, typename Pc>
        double AngleDeg(const Point<Pb, 2>& b, const Point<Pa, 2>& a, const Point<Pc, 2>& c)
        {
            return glm::degrees(AngleRad(b, a, c));
        }

        template<typename Precision>
        struct Rect
        {
            using TypePrecision = Precision;
            using TypePoint = Point<TypePrecision, 2>;
            using TypeVec = Vec<TypePrecision, 2>;

            TypePoint pos;
            TypeVec size;

            TypePoint UpLeft()  const {return pos;}
            TypePoint UpRight() const {return pos + Rect{size.x, 0};}
            TypePoint DnLeft()  const {return pos + Rect{0, size.y};}
            TypePoint DnRight() const {return pos + size.x;}
            TypePoint UpMid()   const {return pos + Rect{size.x / 2, 0};}
            TypePoint DnMid()   const {return pos + Rect{size.x, size.y};}
            TypePoint LeftMid() const {return pos + Rect{0, size.y / 2};}
            TypePoint RightMid()const {return pos + Rect{0, size.y / 2};}
            TypePoint Center()  const {return pos + size / 2;}

            Precision Area() const;
        };

        template<typename Precision>
        struct Circle
        {
            using TypePrecision = Precision;
            using TypePoint = Point<TypePrecision, 2>;
            using TypeVec = Vec<TypePrecision, 2>;
            using TypeRect = Rect<TypePrecision>;

            TypePoint pos;
            TypePrecision r;

            TypePoint Center()  const {return pos;}
            TypePoint Up()      const {return pos - TypeVec{0, r};}
            TypePoint Dn()      const {return pos + TypeVec{0, r};}
            TypePoint Left()    const {return pos - TypeVec{r, 0};}
            TypePoint Right()   const {return pos + TypeVec{0, r};}
            TypeRect Rectangle()const {return TypeRect{TypePoint(pos - TypeVec{r, r}), TypeVec(r, r) * 2};}

            Precision Area() const;
            double AreaReal() const;
        };

        template<typename Precision>
        struct Line
        {
            using TypePrecision = Precision;
            using TypePoint = Point<TypePrecision, 2>;

            TypePoint a, b;
        };

        template<typename Precision>
        struct Ray : Line<Precision>
        {
            double AngleRad() const {return glm::atan(b.y - a.y, b.x - a.x);}
            double AngleDeg() const {return glm::degrees(AngleRad());}
            Ray RayBA() const {return Ray{b, a};}
        };

        template<typename Precision>
        struct Segment : Line<Precision>
        {
            TypePoint Up() const;
            TypePoint Dn() const;
            TypePoint Left() const;
            TypePoint Right() const;
            TypePoint Center() const;
            Precision Length() const;
            Precision LengthSquared() const;
            double LengthReal() const;
            double LengthRealSquared() const;
            const Ray<Precision>& RayAB() const {return *static_cast<Ray<Precision>*>(this);}
        };

        template<typename Precision>
        struct Triangle
        {
            using TypePrecision = Precision;
            using TypePoint = Point<TypePrecision, 2>;
            using TypeVec = Vec<TypePrecision, 2>;
            using TypeRect = Rect<TypePrecision>;
            using TypeSegment = Segment<TypePrecision>;

            TypePoint a, b, c;

            Precision Area() const;
            double AreaReal() const;
        };

        template<typename Precision>
        struct Quadrilateral
        {
            using TypePrecision = Precision;
            using TypePoint = Point<TypePrecision, 2>;
            using TypeVec = Vec<TypePrecision, 2>;
            using TypeRect = Rect<TypePrecision>;
            using TypeSegment = Segment<TypePrecision>;

            TypePoint a, b, c, d;

            Precision Area() const;
            double AreaReal() const;
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

    using Line2i = Generic::Line<int>;
    using Line2f = Generic::Line<float>;
    using Line2d = Generic::Line<double>;
    using Line2 = Generic::Line<DefaultPrecision>;

    using Ray2i = Generic::Ray<int>;
    using Ray2f = Generic::Ray<float>;
    using Ray2d = Generic::Ray<double>;
    using Ray2 = Generic::Ray<DefaultPrecision>;

    using Segment2i = Generic::Segment<int>;
    using Segment2f = Generic::Segment<float>;
    using Segment2d = Generic::Segment<double>;
    using Segment2 = Generic::Segment<DefaultPrecision>;

    using Tri2i = Generic::Triangle<int>;
    using Tri2f = Generic::Triangle<float>;
    using Tri2d = Generic::Triangle<double>;
    using Tri2 = Generic::Triangle<DefaultPrecision>;

    using Quad2i = Generic::Quadrilateral<int>;
    using Quad2f = Generic::Quadrilateral<float>;
    using Quad2d = Generic::Quadrilateral<double>;
    using Quad2 = Generic::Quadrilateral<DefaultPrecision>;
}
