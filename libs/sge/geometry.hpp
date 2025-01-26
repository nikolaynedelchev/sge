#pragma once
#include "geometry.h"

namespace ndn::sge
{
namespace Generic
{
template<typename Pb, typename Pa, typename Pc>
double Angle(const Point<Pb, 2>& b, const Point<Pa, 2>& a, const Point<Pc, 2>& c)
{
    return glm::angle(glm::normalize(b - a), glm::normalize(c - a));
}

template<typename Pb, typename Pa, typename Pc>
double AngleDeg(const Point<Pb, 2>& b, const Point<Pa, 2>& a, const Point<Pc, 2>& c)
{
    return glm::degrees(Angle(b, a, c));
}

// Rect
template<typename Precision>
typename Rect<Precision>::TypePoint Rect<Precision>::UpLeft()  const
{
    return pos;
}
template<typename Precision>
typename Rect<Precision>::TypePoint Rect<Precision>::UpRight() const
{
    return this->pos + Rect<Precision>{this->size.x, 0};
}
template<typename Precision>
typename Rect<Precision>::TypePoint Rect<Precision>::DnLeft()  const
{
    return this->pos + Rect<Precision>{0, this->size.y};
}
template<typename Precision>
typename Rect<Precision>::TypePoint Rect<Precision>::DnRight() const
{
    return this->pos + this->size.x;
}
template<typename Precision>
typename Rect<Precision>::TypePoint Rect<Precision>::UpMid() const
{
    return this->pos + Rect<Precision>{this->size.x / 2, 0};
}
template<typename Precision>
typename Rect<Precision>::TypePoint Rect<Precision>::DnMid() const
{
    return this->pos + Rect<Precision>{this->size.x, this->size.y};
}
template<typename Precision>
typename Rect<Precision>::TypePoint Rect<Precision>::LeftMid() const
{
    return this->pos + Rect<Precision>{0, this->size.y / 2};
}
template<typename Precision>
typename Rect<Precision>::TypePoint Rect<Precision>::RightMid()const
{
    return this->pos + Rect<Precision>{0, this->size.y / 2};
}
template<typename Precision>
typename Rect<Precision>::TypePoint Rect<Precision>::Center()  const
{
    return this->pos + this->size / 2;
}
template<typename Precision>
Precision Rect<Precision>::Area() const
{
    return this->size.x * this->size.y;
}

// Circle
template<typename Precision>
typename Circle<Precision>::TypePoint Circle<Precision>::TypePointCenter()  const
{
    return pos;
}
template<typename Precision>
typename Circle<Precision>::TypePoint Circle<Precision>::TypePointUp()      const
{
    return pos - TypeVec{0, r};
}
template<typename Precision>
typename Circle<Precision>::TypePoint Circle<Precision>::TypePointDn()      const
{
    return pos + TypeVec{0, r};
}
template<typename Precision>
typename Circle<Precision>::TypePoint Circle<Precision>::TypePointLeft()    const
{
    return pos - TypeVec{r, 0};
}
template<typename Precision>
typename Circle<Precision>::TypePoint Circle<Precision>::TypePointRight()   const
{
    return pos + TypeVec{0, r};
}
template<typename Precision>
typename Circle<Precision>::TypeRect Circle<Precision>::TypePointRectangle()const
{
    return TypeRect{TypePoint(pos - TypeVec{r, r}), TypeVec(r, r) * 2};
}
template<typename Precision>
double Circle<Precision>::Area() const
{
    return this->r * this->r * Pi;
}

// Line
template<typename Precision>
void Line<Precision>::Rotate(double radians, const TypePoint& basis)
{
    auto m = RotationMatrix2(radians);
    a = (m * (a - basis)) + basis;
    b = (m * (b - basis)) + basis;
}
template<typename Precision>
void Line<Precision>::RotateDeg(double degrees, const TypePoint& basis)
{
    Rotate(glm::radians(degrees));
}
template<typename Precision>
void Line<Precision>::Scale(double scale, const TypePoint& basis)
{
    a = ((a - basis) * scale) + basis;
    b = ((b - basis) * scale) + basis;
}

template<typename Precision>
const Ray<Precision>& Line<Precision>::AsRay() const
{
    return *static_cast<const Ray<Precision>*>(this);
}
template<typename Precision>
const Segment<Precision>& Line<Precision>::AsSegment() const
{
    return *static_cast<const Segment<Precision>*>(this);
}
template<typename Precision>
Ray<Precision>& Line<Precision>::AsRay()
{
    return *static_cast<Ray<Precision>*>(this);
}
template<typename Precision>
Segment<Precision>& Line<Precision>::AsSegment()
{
    return *static_cast<Segment<Precision>*>(this);
}

// Ray
template<typename Precision>
double Ray<Precision>::Angle() const
{
    return glm::atan(this->b.y - this->a.y, this->b.x - this->a.x);
}
template<typename Precision>
double Ray<Precision>::AngleDeg() const
{
    return glm::radians(this->Angle());
}
template<typename Precision>
const Segment<Precision>& Ray<Precision>::AsSegment() const
{
    return *static_cast<const Segment<Precision>*>(this);
}
template<typename Precision>
Segment<Precision>& Ray<Precision>::AsSegment()
{
    return *static_cast<Segment<Precision>*>(this);
}

// Segment
template<typename Precision>
double Segment<Precision>::Length() const
{
    return glm::distance(double(this->a), double(this->b));
}
template<typename Precision>
double Segment<Precision>::LengthSquared() const
{
    return glm::distance(double(this->a), double(this->b));
}
template<typename Precision>
const Ray<Precision>& Segment<Precision>::AsRay() const
{
    return *static_cast<const Ray<Precision>*>(this);
}
template<typename Precision>
Ray<Precision>& Segment<Precision>::AsRay()
{
    return *static_cast<Ray<Precision>*>(this);
}

// Triangle
template<typename Precision>
double Triangle<Precision>::Area() const
{
    return 0.5 * glm::abs(
               a.x * (b.y - c.y) +
               b.x * (c.y - a.y) +
               c.x * (a.y - b.y)
               );
}
template<typename Precision>
void Triangle<Precision>::Rotate(double radians, const Triangle<Precision>::TypePoint& basis)
{
    auto m = RotationMatrix2(radians);
    a = (m * (a - basis)) + basis;
    b = (m * (b - basis)) + basis;
    c = (m * (c - basis)) + basis;
}
template<typename Precision>
void Triangle<Precision>::RotateDeg(double degrees, const Triangle<Precision>::TypePoint& basis)
{
    Rotate(glm::radians(degrees), basis);
}
template<typename Precision>
void Triangle<Precision>::Scale(double scale, const Triangle<Precision>::TypePoint& basis)
{
    a = ((a - basis) * scale) + basis;
    b = ((b - basis) * scale) + basis;
    c = ((c - basis) * scale) + basis;
}

// Quadrilateral
template<typename Precision>
double Quadrilateral<Precision>::Area() const
{
    return 0.5 * std::abs(
               a.x * b.y + b.x * c.y + c.x * d.y + d.x * a.y -
               (a.y * b.x + b.y * c.x + c.y * d.x + d.y * a.x)
               );
}
}
}
