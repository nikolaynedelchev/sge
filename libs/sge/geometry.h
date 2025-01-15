#pragma once
#include "types.h"

namespace ndn::sge::Geom
{


// Rect
template<typename Precision>
Generic::Point<Precision, 2> UpLeft(const Generic::Rect<Precision>& rect){return rect.pos;};
template<typename Precision>
Generic::Point<Precision, 2> UpRight(const Generic::Rect<Precision>& rect){return rect.pos + Generic::Rect<Precision>{rect.size.x, 0};};
template<typename Precision>
Generic::Point<Precision, 2> DnLeft(const Generic::Rect<Precision>& rect) {return rect.pos + Generic::Rect<Precision>{0, rect.size.y};};
template<typename Precision>
Generic::Point<Precision, 2> DnRight(const Generic::Rect<Precision>& rect) {return rect.pos + rect.size.x;};
template<typename Precision>
Generic::Point<Precision, 2> UpMid(const Generic::Rect<Precision>& rect) {return rect.pos + Generic::Rect<Precision>{rect.size.x / 2, 0};};
template<typename Precision>
Generic::Point<Precision, 2> DnMid(const Generic::Rect<Precision>& rect) {return rect.pos + Generic::Rect<Precision>{rect.size.x, rect.size.y};};
template<typename Precision>
Generic::Point<Precision, 2> LeftMid(const Generic::Rect<Precision>& rect) {return rect.pos + Generic::Rect<Precision>{0, rect.size.y / 2};};
template<typename Precision>
Generic::Point<Precision, 2> RightMid(const Generic::Rect<Precision>& rect) {return rect.pos + Generic::Rect<Precision>{0, rect.size.y / 2};};
template<typename Precision>
Generic::Point<Precision, 2> Center(const Generic::Rect<Precision>& rect) {return rect.pos + rect.size / 2;};

// Circle
template<typename Precision>
Generic::Point<Precision, 2> Center(const Generic::Circle<Precision>& c) {return c.pos;};
template<typename Precision>
Generic::Point<Precision, 2> Up(const Generic::Circle<Precision>& c) {return c.pos - Generic::Vec<Precision, 2>{0, c.r};};
template<typename Precision>
Generic::Point<Precision, 2> Dn(const Generic::Circle<Precision>& c) {return c.pos + Generic::Vec<Precision, 2>{0, c.r};};
template<typename Precision>
Generic::Point<Precision, 2> Left(const Generic::Circle<Precision>& c) {return c.pos - Generic::Vec<Precision, 2>{c.r, 0};};
template<typename Precision>
Generic::Point<Precision, 2> Right(const Generic::Circle<Precision>& c) {return c.pos + Generic::Vec<Precision, 2>{0, c.r};};
template<typename Precision>
Generic::Rect<Precision> Rectangle(const Generic::Circle<Precision>& c) {return Generic::Rect<Precision>{Generic::Point<Precision, 2>(c.pos - Generic::Vec<Precision, 2>{c.r, c.r}), Generic::Vec<Precision, 2>(2 * c.r, 2 * c.r)};};


}
