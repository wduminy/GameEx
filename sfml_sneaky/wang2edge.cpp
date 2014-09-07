// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include "wang2edge.h"
#include "exception.h"
namespace codespear {

Wang2EdgeField::Wang2EdgeField(size_t width, size_t height, const std::vector<size_t>& data)
 : m_width(width) , m_height(height), m_data(data) {
	ASSERT(m_data.size() == m_height * m_width);
}

/**
 * The visitor v is called for every position in the field. Visiting order is
 * (0,0), (0,1) ... (1,0), (1,0) ... (width-1,height-1)
 * @param v
 */
void Wang2EdgeField::visit(std::function<void(const Tile& t)> v) const {
	Tile t;
	for (t.at.x = 0; t.at.x < m_width; t.at.x++)
		for (t.at.y = 0; t.at.y < m_height; t.at.y++) {
			t.index = t.at.x + t.at.y * m_width;
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
		quad[0].position = t.at * world_size;
		quad[1].position = quad[0].position + sf::Vector2f{world_size,0.f};
		quad[2].position = quad[0].position + sf::Vector2f{world_size,world_size};
		quad[3].position = quad[0].position + sf::Vector2f{0.f,world_size};
		quad[0].texCoords = tex_location(t.tile,4,tex_size_px);
		quad[1].texCoords = quad[0].texCoords + sf::Vector2f{tex_size_px,0.f};
		quad[2].texCoords = quad[0].texCoords + sf::Vector2f{tex_size_px,tex_size_px};
		quad[3].texCoords = quad[0].texCoords + sf::Vector2f{0.f,tex_size_px};
	});
}

#define DRAW_CENTER w.add_static_rect({t.at.x*size+box_size*1.5f,t.at.y*size+box_size*1.5f},	box_size,box_size)

#define DRAW_4 w.add_static_rect({t.at.x*size+box_size*1.5f,t.at.y*size+size/2.f},	box_size,size)
#define DRAW_14 w.add_static_rect({t.at.x*size+size/2.0f, t.at.y*size+box_size*1.5f},size,box_size)
#define DRAW_13 w.add_static_rect({t.at.x*size+box_size*2.f, t.at.y*size+box_size*1.5f},box_size*2.f,box_size)
#define DRAW_8 w.add_static_rect({t.at.x*size+box_size*1.5f,t.at.y*size+box_size*1.f},	box_size,box_size*2.f)
#define DRAW_15 w.add_static_rect({t.at.x*size+box_size*1.f, t.at.y*size+box_size*1.5f},box_size*2.f,box_size)
#define DRAW_0 w.add_static_rect({t.at.x*size+box_size*1.5f,t.at.y*size+box_size*2.f},	box_size,box_size*2.f)

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

bool Wang2EdgeField::in_range(const UIntVector& v) const {
	return (v.x < m_width) && (v.y < m_height);
}

size_t Wang2EdgeField::tile_at(const UIntVector& v) const {
	ASSERT(in_range(v));
	return m_data[v.x + v.y * m_width];
}

}
