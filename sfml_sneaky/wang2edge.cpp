// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include "wang2edge.h"

namespace codespear {

Wang2EdgeField::Wang2EdgeField(size_t width, size_t height, std::vector<size_t> data)
 : m_width(width) , m_height(height), m_data(data) {

}

void Wang2EdgeField::visit(std::function<void(const Tile& t)> v) {
	Tile t;
	for (t.x = 0; t.x < m_width; t.x++)
		for (t.y = 0; t.y < m_height; t.y++) {
			t.index = t.x + t.y * m_width;
			t.tile = m_data[t.index];
			v(t);
		}
}

sf::Vector2f tex_location(const int index, const int tiles_in_row, const float tile_size) {
	const int y = index / tiles_in_row;
	const int x = index - tiles_in_row * y;
	return sf::Vector2f(x * tile_size,y * tile_size);
}


void Wang2EdgeField::fill_vertices(float tex_size_px, float world_size, sf::Vertex * target) {
	visit([&](const Tile &t) {
		sf::Vertex * quad = &target[t.index*4];
		quad[0].position = sf::Vector2f{t.x*world_size,t.y*world_size};
		quad[1].position = quad[0].position + sf::Vector2f{world_size,0.f};
		quad[2].position = quad[0].position + sf::Vector2f{world_size,world_size};
		quad[3].position = quad[0].position + sf::Vector2f{0.f,world_size};
		quad[0].texCoords = tex_location(t.tile,4,tex_size_px);
		quad[1].texCoords = quad[0].texCoords + sf::Vector2f{tex_size_px,0.f};
		quad[2].texCoords = quad[0].texCoords + sf::Vector2f{tex_size_px,tex_size_px};
		quad[3].texCoords = quad[0].texCoords + sf::Vector2f{0.f,tex_size_px};
	});
}

#define DRAW_CENTER w.add_static_rect(t.x*size+box_size*1.5f,t.y*size+box_size*1.5f,	box_size,box_size)

#define DRAW_4 w.add_static_rect(t.x*size+box_size*1.5f,t.y*size+size/2.f,	box_size,size)
#define DRAW_14 w.add_static_rect(t.x*size+size/2.0f, t.y*size+box_size*1.5f,size,box_size)
#define DRAW_13 w.add_static_rect(t.x*size+box_size*2.f, t.y*size+box_size*1.5f,box_size*2.f,box_size)
#define DRAW_8 w.add_static_rect(t.x*size+box_size*1.5f,t.y*size+box_size*1.f,	box_size,box_size*2.f)
#define DRAW_15 w.add_static_rect(t.x*size+box_size*1.f, t.y*size+box_size*1.5f,box_size*2.f,box_size)
#define DRAW_0 w.add_static_rect(t.x*size+box_size*1.5f,t.y*size+box_size*2.f,	box_size,box_size*2.f)

void Wang2EdgeField::fill_world(float size, PhysicsWorld& w) {
	const float box_size = size / 3.f;
	visit([&](const Tile &t) {
		switch (t.tile) {
		case 0:
			DRAW_0;
			break;
		case 4:
			DRAW_4;
			break;
		case 8:
			DRAW_8;
			break;
		case 12:
			break;
		case 13:
			DRAW_13;
			break;
		case 14:
			DRAW_14;
			break;
		case 15:
			DRAW_15;
			break;
		default:
			DRAW_CENTER;
			break;
		}
	});
}


}
