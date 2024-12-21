#pragma once
#include "engine_bridge.h"

// Points
ndn::sge::point& operator+=(ndn::sge::point& v1, const ndn::sge::point& v2);
ndn::sge::point operator+(const ndn::sge::point& v1, const ndn::sge::point& v2);
ndn::sge::point& operator-=(ndn::sge::point& v1, const ndn::sge::point& v2);
ndn::sge::point operator-(const ndn::sge::point& v1, const ndn::sge::point& v2);
ndn::sge::point& operator*=(ndn::sge::point& v1, const ndn::sge::point& v2);
ndn::sge::point operator*(const ndn::sge::point& v1, const ndn::sge::point& v2);
ndn::sge::point& operator/=(ndn::sge::point& v1, const ndn::sge::point& v2);
ndn::sge::point operator/(const ndn::sge::point& v1, const ndn::sge::point& v2);
ndn::sge::point& operator+=(ndn::sge::point& v1, float v2);
ndn::sge::point operator+(const ndn::sge::point& v1, float v2);
ndn::sge::point& operator-=(ndn::sge::point& v1, float v2);
ndn::sge::point operator-(const ndn::sge::point& v1, float v2);
ndn::sge::point& operator*=(ndn::sge::point& v1, float v2);
ndn::sge::point operator*(const ndn::sge::point& v1, float v2);
ndn::sge::point& operator/=(ndn::sge::point& v1, float v2);
ndn::sge::point operator/(const ndn::sge::point& v1, float v2);
bool operator==(const ndn::sge::point& v1, const ndn::sge::point v2);
//
ndn::sge::point& operator+=(float v1, ndn::sge::point& v2);
ndn::sge::point operator+(float v1, ndn::sge::point& v2);
ndn::sge::point& operator-=(float v1, ndn::sge::point& v2);
ndn::sge::point operator-(float v1, ndn::sge::point& v2);
ndn::sge::point& operator*=(float v1, ndn::sge::point& v2);
ndn::sge::point operator*(float v1, ndn::sge::point& v2);
ndn::sge::point& operator/=(float v1, ndn::sge::point& v2);
ndn::sge::point operator/(float v1, ndn::sge::point& v2);

// Rects
ndn::sge::rect& operator+=(ndn::sge::rect& r1, const ndn::sge::point& v2);
ndn::sge::rect operator+(const ndn::sge::rect& r1, const ndn::sge::point& v2);
ndn::sge::rect& operator-=(ndn::sge::rect& r1, const ndn::sge::point& v2);
ndn::sge::rect operator-(const ndn::sge::rect& r1, const ndn::sge::point& v2);

// Circles
// ndn::sge::circle& operator+=(ndn::sge::circle& c1, const ndn::sge::point& v2);
// ndn::sge::circle operator+(const ndn::sge::circle& c1, const ndn::sge::point& v2);
// ndn::sge::circle& operator-=(ndn::sge::circle& c1, const ndn::sge::point& v2);
// ndn::sge::circle operator-(const ndn::sge::circle& c1, const ndn::sge::point& v2);
// ndn::sge::circle& operator*=(ndn::sge::circle& c1, const float& v2);
// ndn::sge::circle operator*(const ndn::sge::circle& c1, const float& v2);
// ndn::sge::circle& operator/=(ndn::sge::circle& c1, const float& v2);
// ndn::sge::circle operator/(const ndn::sge::circle& c1, const float& v2);
