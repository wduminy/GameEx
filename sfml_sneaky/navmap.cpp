// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include "navmap.h"
#include "exception.h"
#include "log.h"
#include <boost/graph/astar_search.hpp>
namespace {
struct DebugDrawer {
float scale {1.f};
sf::RenderStates states;
sf::RenderTarget * t {nullptr};

void segment(const codespear::MeterVector& p1, const codespear::MeterVector& p2){
	sf::VertexArray a(sf::Lines, 2);
	a[0].position = sf::Vector2f(p1.x*scale, p1.y*scale);
	a[1].position = sf::Vector2f(p2.x*scale, p2.y*scale);
	a[0].color = sf::Color::Red;
	a[1].color = sf::Color::Red;
	t->draw(a,states);
};

void circle(const codespear::MeterVector& center) {
	const float radius = 0.1f;
	sf::CircleShape s;
	s.setPosition({center.x*scale,center.y*scale});
	s.setOutlineColor(sf::Color::Green);
    s.setOrigin({radius*scale,radius*scale});
	s.setRadius(radius*scale);
	s.setFillColor({0,0,0,0});
	s.setOutlineThickness(3);
	t->draw(s,states);
};

} drawer;
};
namespace codespear {

void add_edge(NavigationMap::vertex_idx_t& a,
		NavigationMap::vertex_idx_t& b,
		NavigationMap::graph_t& g, float32 distance) {
	if (!boost::edge(a,b,g).second) {
		auto e = boost::add_edge(a,b,g).first;
		boost::get(boost::edge_weight,g)[e] = distance;
	}
}

void NavigationMap::add_relative(const PathSegment& s, bool bi_directional) {
	auto distance = s.b.LengthSquared();
	auto ia = make_index(s.a);
	auto ib = make_index(s.a+s.b);
	ASSERT(ia != ib);
	add_edge(ia,ib,m_graph,distance);
	if (bi_directional)
		add_edge(ib,ia,m_graph,distance);
}

void NavigationMap::add_absolute(const PathSegment& s, bool bi_directional) {
	TRACE << "adding " << s;
	auto distance = s.distance_squared();
	auto ia = make_index(s.a);
	auto ib = make_index(s.b);
	ASSERT(ia != ib);
	add_edge(ia,ib,m_graph,distance);
	if (bi_directional)
		add_edge(ib,ia,m_graph,distance);
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

class DistanceSquaredHeuristic : public boost::astar_heuristic<NavigationMap::graph_t, float32>
{
public:
	DistanceSquaredHeuristic(const NavigationMap::graph_t &g, const MeterVector& goal)
    : m_graph(g), m_goal(goal) {}
  float32 operator()(NavigationMap::vertex_idx_t u)
  {
	 return (m_goal - m_graph[u]).LengthSquared();
  }
private:
  const NavigationMap::graph_t &m_graph;
  const MeterVector m_goal;
};

class GoalFoundException {};

class AStarGoalVisitor : public boost::default_astar_visitor
{
public:
  AStarGoalVisitor(NavigationMap::vertex_idx_t goal) : m_goal(goal) {}
  void examine_vertex(NavigationMap::vertex_idx_t u, const NavigationMap::graph_t& g) {
    if(u == m_goal)
      throw GoalFoundException();
  }
private:
  NavigationMap::vertex_idx_t m_goal;
};

std::list<MeterVector> NavigationMap::astar(const MeterVector& a, const MeterVector & b) const {
	using namespace boost;
	std::list<MeterVector>  result;
	auto ia = m_vertex_indexes.at(a);
	auto ib = m_vertex_indexes.at(b);
	std::vector<vertex_idx_t> preds(num_vertices(m_graph));
	// call boost a-star
	try {
		astar_search(m_graph,ia,DistanceSquaredHeuristic(m_graph,b),
				predecessor_map(make_iterator_property_map(preds.begin(), get(vertex_index, m_graph))).
				visitor(AStarGoalVisitor(ib)));
	} catch (GoalFoundException &) {
		// boost uses an exception to interrupt a successful search.
		// i cannot say I like that, but here goes
		 for(auto v = ib;;v = preds[v]) {
		      result.push_front(m_graph[v]);
		      if(preds[v] == v)
		    	  break; // end of path
		 }
	}
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


void NavigationMap::debug_draw(sf::RenderTarget &t,const float pixels_per_meter, sf::RenderStates states) const {
	drawer.scale = pixels_per_meter;
	drawer.states = states;
	drawer.t = &t;
	drawer.states.blendMode  = sf::BlendNone;
	for (const auto& e : m_vertex_indexes) {
		drawer.circle(e.first);
		for (const auto& t : from(e.first))
			drawer.segment(e.first, t);
	}
}


}

std::ostream& operator <<(std::ostream& os, const codespear::PathSegment& v) {
	return os << "(" << v.a << "->" << v.b << ")";
}

