// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#pragma once
#include <Box2D/Box2D.h>
#include "types.h"
#include <ostream>

namespace codespear {

using MeterVector = b2Vec2; // a position in the physical world: 1.0f equals 1 meter
extern const MeterVector EAST;
extern const MeterVector WEST;
extern const MeterVector NORTH;
extern const MeterVector SOUTH;
extern const MeterVector ORIGIN;
#define UP NORTH
#define DOWN SOUTH
#define LEFT WEST
#define RIGHT EAST


}
std::ostream& operator<<(std::ostream& s, const codespear::MeterVector& v);
bool operator<(const codespear::MeterVector& a, const codespear::MeterVector& b);

