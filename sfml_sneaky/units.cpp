// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0
#include "units.h"
#include "log.h"

namespace codespear {
	const MeterVector EAST{-1.f,0.f};
	const MeterVector WEST{1.f,0.f};
	const MeterVector NORTH{0.f,1.f};
	const MeterVector SOUTH{0.f,-1.f};
	const MeterVector ORIGIN(0.f,0.f);
}

std::ostream& operator<<(std::ostream& os, const codespear::MeterVector& v) {
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}

bool operator<(const codespear::MeterVector& a, const codespear::MeterVector& b) {
	bool result = false;
	if (a.x == b.x)
		result = a.y < b.y;
	else
		result = a.x < b.x;
	return result;
}


