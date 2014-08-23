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
	float32 distance_squared() const {return (a-b).LengthSquared();}
};

class NavigationMap {
public:
	typedef boost::property<boost::edge_weight_t, float32> edge_t;
	typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, MeterVector, edge_t > graph_t;
	typedef graph_t::vertex_descriptor vertex_idx_t;
private:
	graph_t m_graph;
	std::map<MeterVector,vertex_idx_t> m_vertex_indexes;
public:
	/** add from a to (a + b) */
	void add_relative(const PathSegment& s, bool bi_directional = true);
	/** add from a to b **/
	void add_absolute(const PathSegment& s, bool bi_directional = true);
	/** find all the points reachable from v*/
	std::vector<MeterVector> from(const MeterVector& v) const;
	/** find a short path from a to b */
	std::list<MeterVector> astar(const MeterVector& a, const MeterVector& b) const;
private:
	/** makes an index if one does not exist */
	auto make_index(const MeterVector &v) -> vertex_idx_t;
};


}
