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
	if (!boost::edge(ia,ib,m_graph).second)
		boost::add_edge(ia,ib,m_graph);
	if (bi_directional)
		if (!boost::edge(ib,ia,m_graph).second)
			boost::add_edge(ib,ia,m_graph);
}

std::vector<MeterVector> NavigationMap::from(const MeterVector& v) const {
	boost::graph_traits<graph_t>::adjacency_iterator it_b, it_e;
	std::vector<MeterVector> result;
	auto ix = m_vertex_indexes.at(v);
	boost::tie(it_b, it_e) = boost::adjacent_vertices(ix,m_graph);
	for (auto it = it_b;it != it_e; it++)
		result.push_back(m_graph[*it]);
	return result;
}


auto NavigationMap::make_index(const MeterVector &v) -> vertex_idx_t {
	auto fnd = m_vertex_indexes.find(v);
	if (fnd != m_vertex_indexes.end())
		return fnd->second;
	else {
		auto ni = boost::add_vertex(m_graph);
		m_graph[ni] = v;
		m_vertex_indexes[v] = ni;
		return ni;
	}
}

}
