// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include "navmap.h"
#include "exception.h"
#include "log.h"

namespace codespear {

void NavigationMap::add_relative(const PathSegment& s, bool bi_directional) {
	auto const b = s.a+s.b;
	auto ia = make_index(s.a);
	auto ib = make_index(b);
	ASSERT(ia != ib);
	m_branches.at(ia).insert(ib);
	if (bi_directional) {
		m_branches.at(ib).insert(ia);
	}
}

std::vector<MeterVector> NavigationMap::from(const MeterVector& v) const {
	std::vector<MeterVector> result;
	auto iv = m_indexes.at(v);
	for (auto i : m_branches.at(iv))
		result.push_back(m_points[i]);
	return result;
}

size_t NavigationMap::make_index(const MeterVector &v) {
	auto fnd = m_indexes.find(v);
	size_t result;
	if (fnd != m_indexes.end())
		result = fnd->second;
	else {
		m_points.push_back(v);
		result = m_points.size()-1;
		m_indexes[v] = result;
		m_branches.insert({result,std::set<size_t>()});
	}

	return result;
}

}
