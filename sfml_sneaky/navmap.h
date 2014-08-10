// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#pragma once
#include "units.h"
#include "container.h"
#include <map>
#include <set>

namespace codespear {

struct PathSegment {
	MeterVector a;
	MeterVector b;
};

class NavigationMap {
private:
	std::map<MeterVector,size_t> m_indexes;
	std::map<size_t,std::set<size_t>> m_branches;
	std::vector<MeterVector> m_points;
public:
	/** add from a to (a + b) */
	void add_relative(const PathSegment& s, bool bi_directional = true);
	/** find all the points reachable from v*/
	std::vector<MeterVector> from(const MeterVector& v) const;
private:
	/** makes an index if one does not exist */
	size_t make_index(const MeterVector &v);
};


}
