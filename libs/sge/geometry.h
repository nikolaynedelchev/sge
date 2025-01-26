#pragma once
#include <glm/glm.hpp>
#include <glm/geometric.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

namespace ndn::sge
{

constexpr double Eps = 1e-6f;
constexpr double Pi = 3.1415926535897932384626433832795;
glm::mat2 RotationMatrix2(double radians);

using DefaultPrecision = double;

namespace Generic
{
    template<typename Precision, int Dimensions>
    using Vec = glm::vec<Dimensions, Precision, glm::defaultp>;

    template<typename Precision, int Dimensions>
    using Point = Vec<Precision, Dimensions>;

    template<typename Pb, typename Pa, typename Pc>
    double Angle(const Point<Pb, 2>& b, const Point<Pa, 2>& a, const Point<Pc, 2>& c);

    template<typename Pb, typename Pa, typename Pc>
    double AngleDeg(const Point<Pb, 2>& b, const Point<Pa, 2>& a, const Point<Pc, 2>& c);

    template<typename Precision>
    struct Rect
    {
        using TypePrecision = Precision;
        using TypePoint = Point<TypePrecision, 2>;
        using TypeVec = Vec<TypePrecision, 2>;

        TypePoint pos;
        TypeVec size;

        TypePoint UpLeft()  const;
        TypePoint UpRight() const;
        TypePoint DnLeft()  const;
        TypePoint DnRight() const;
        TypePoint UpMid()   const;
        TypePoint DnMid()   const;
        TypePoint LeftMid() const;
        TypePoint RightMid()const;
        TypePoint Center()  const;

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

        TypePoint Center()  const;
        TypePoint Up()      const;
        TypePoint Dn()      const;
        TypePoint Left()    const;
        TypePoint Right()   const;
        TypeRect Rectangle()const;

        double Area() const;
    };

    template<typename Precision>struct Ray;
    template<typename Precision>struct Segment;

    template<typename Precision>
    struct Line
    {
        using TypePrecision = Precision;
        using TypePoint = Point<TypePrecision, 2>;

        TypePoint a;
        TypePoint b;

        void Rotate(double radians, const TypePoint& basis = TypePoint{0,0});
        void RotateDeg(double degrees, const TypePoint& basis = TypePoint{0,0});
        void Scale(double scale, const TypePoint& basis = TypePoint{0,0});

        const Ray<Precision>& AsRay() const;
        const Segment<Precision>& AsSegment() const;

        Ray<Precision>& AsRay();
        Segment<Precision>& AsSegment();
    };

    template<typename Precision>
    struct Ray : Line<Precision>
    {
        double Angle() const;
        double AngleDeg() const;

        const Segment<Precision>& AsSegment() const;
        Segment<Precision>& AsSegment();
    };

    template<typename Precision>
    struct Segment : Line<Precision>
    {
        double Length() const;
        double LengthSquared() const;

        const Ray<Precision>& AsRay() const;
        Ray<Precision>& AsRay();
    };

    template<typename Precision>
    struct Triangle
    {
        using TypePrecision = Precision;
        using TypePoint = Point<TypePrecision, 2>;
        using TypeVec = Vec<TypePrecision, 2>;
        using TypeRect = Rect<TypePrecision>;
        using TypeLine = Line<TypePrecision>;

        TypePoint a, b, c;

        double Area() const;

        void Rotate(double radians, const TypePoint& basis = TypePoint{0,0});
        void RotateDeg(double degrees, const TypePoint& basis = TypePoint{0,0});
        void Scale(double scale, const TypePoint& basis = TypePoint{0,0});
    };

    template<typename Precision>
    struct Quadrilateral
    {
        using TypePrecision = Precision;
        using TypePoint = Point<TypePrecision, 2>;
        using TypeVec = Vec<TypePrecision, 2>;
        using TypeRect = Rect<TypePrecision>;
        using TypeLine = Line<TypePrecision>;

        TypePoint a, b, c, d;

        double Area() const;

        void Rotate(double radians, const TypePoint& basis = TypePoint{0,0});
        void RotateDeg(double degrees, const TypePoint& basis = TypePoint{0,0});
        void Scale(double scale, const TypePoint& basis = TypePoint{0,0});
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

using Tri2i = Generic::Triangle<int>;
using Tri2f = Generic::Triangle<float>;
using Tri2d = Generic::Triangle<double>;
using Tri2 = Generic::Triangle<DefaultPrecision>;

using Quad2i = Generic::Quadrilateral<int>;
using Quad2f = Generic::Quadrilateral<float>;
using Quad2d = Generic::Quadrilateral<double>;
using Quad2 = Generic::Quadrilateral<DefaultPrecision>;

}
