// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include <vector>
#include <ostream>
#pragma once

template<class T>
inline std::ostream& operator <<(std::ostream& os, const std::vector<T>& v) {
	os << "{";
	for (auto &e : v) os << e << " ";
	os << "}";
	return os;
}

