// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include "sneakyworld.h"
#include "exception.h"
#include "flaremap.h"

namespace sneaky {
using std::vector;
using Point = sf::Vector2f;
using sf::Vertex;
using codespear::FlareMap;

const auto ARENA_SQUARE = 100.f;
const size_t ARENA_WIDTH_TILES = 10;
const size_t ARENA_HEIGHT_TILES = 10;
const size_t ARENA_TILE_COUNT = ARENA_WIDTH_TILES * ARENA_HEIGHT_TILES;
const char* TEXTURE_FILE_NAME = "media/sneaky/tiles.png";
const char* FLAREMAP_FILE_NAME = "media/sneaky/flaremap.txt";

Point tile_location(const int x, const int y) {
	return Point{x*ARENA_SQUARE,y*ARENA_SQUARE};
}

Point tex_location(const int index, const int tiles_in_row, const float tile_size) {
	const int y = index / tiles_in_row;
	const int x = index - tiles_in_row * y;
	return Point(x * tile_size,y * tile_size);
}

size_t tile_number(const int x, const int y) {
	return x + y * ARENA_WIDTH_TILES;
}

class Arena : public SceneNode {
private:
	sf::VertexArray m_vertices{sf::Quads,ARENA_TILE_COUNT*4};
	sf::Texture m_tileTexture;
public:
	Arena() {
		FlareMap fm(FLAREMAP_FILE_NAME);
		auto ixs = fm.layer().at("Level1");
		check_that(m_tileTexture.loadFromFile(TEXTURE_FILE_NAME));
		for (size_t x = 0; x < ARENA_WIDTH_TILES; x++)
			for (size_t y = 0; y < ARENA_HEIGHT_TILES; y++) {
				const auto index = tile_number(x,y)*4;
				Vertex * quad = &m_vertices[index];
				quad[0].position = tile_location(x,y);
				quad[1].position = tile_location(x+1,y);
				quad[2].position = tile_location(x+1,y+1);
				quad[3].position = tile_location(x,y+1);
				auto tile_index = ixs.at(tile_number(x,y)) - 1; // the flare map stores at 1 offset
				quad[0].texCoords = tex_location(tile_index,4,30.f);
				quad[1].texCoords = quad[0].texCoords + Point{29,0};
				quad[2].texCoords = quad[0].texCoords + Point{29,29};
				quad[3].texCoords = quad[0].texCoords + Point{0,29};
			}
	}
protected:
	void draw_node(sf::RenderTarget& target, sf::RenderStates state) const override {
		// TODO 100 define arena concept further
		state.texture = &m_tileTexture;
		target.draw(m_vertices,state);
	}
};

SneakyWorld::SneakyWorld() : m_scene_graph(), m_arena(nullptr) {
}

void SneakyWorld::init() {
	m_scene_graph.attach(SceneNode::u_ptr(m_arena = new Arena()));
}

SneakyGame::SneakyGame() : Game(800,600,"Sneaky"), m_view(window().getDefaultView()) {}

void SneakyGame::init() {
	m_world.init();
}
void SneakyGame::update(codespear::FrameTime step) {
	m_view.move(1,1);
}
void SneakyGame::draw(sf::RenderTarget &window) {
	window.setView(m_view);
	m_world.scene().draw(window,m_rstate);
}
}
