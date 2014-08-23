// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#pragma once
#include "units.h"
#include "container.h"
#include <map>
#include <set>
#include <boost/graph/adjacency_list.hpp>

namespace codespear {

struct PathSegment {
	MeterVector a;
	MeterVector b;
};

class NavigationMap {
private:
	typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, MeterVector> graph_t;
	typedef graph_t::vertex_descriptor vertex_idx_t;
	graph_t m_graph;
	std::map<MeterVector,vertex_idx_t> m_vertex_indexes;
public:
	/** add from a to (a + b) */
	void add_relative(const PathSegment& s, bool bi_directional = true);
	/** find all the points reachable from v*/
	std::vector<MeterVector> from(const MeterVector& v) const;
private:
	/** makes an index if one does not exist */
	auto make_index(const MeterVector &v) -> vertex_idx_t;
};


}
