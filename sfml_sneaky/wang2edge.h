// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#pragma once
#include <vector>
#include <functional>
#include <SFML/Graphics.hpp>
#include "physics.h"

namespace codespear {

struct Tile {
	size_t x,y,index,tile;
};
/**
 * A Wang2EdgeField is a tile map using
 * 16 tiles.  The tile set is layout as site
 * http://www.cr31.co.uk/stagecast/info/tiles_e.html,
 * but numbered in western reading order, starting top-lef as zero
 */
class Wang2EdgeField {
private:
	size_t m_width, m_height;
	std::vector<size_t> m_data;
public:
	Wang2EdgeField(size_t width, size_t height, std::vector<size_t> data);
	void fill_vertices(float tex_size_px, float world_size, sf::Vertex * target);
	void fill_world(float size, PhysicsWorld& w);
private:
	void visit(std::function<void(const Tile& t)> v);

};

}
