// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include "sneakyworld.h"

namespace sneaky {
using std::vector;
using Location = sf::Vector2f;
using sf::Vertex;

const auto ARENA_SQUARE = 100.f;
const size_t ARENA_WIDTH_TILES = 30;
const size_t ARENA_HEIGHT_TILES = 30;
const size_t ARENA_TILE_COUNT = ARENA_WIDTH_TILES * ARENA_HEIGHT_TILES;

Location tile_location(const int x, const int y) {
	return Location{x*ARENA_SQUARE,y*ARENA_SQUARE};
}

size_t tile_number(const int x, const int y) {
	return x + y * ARENA_WIDTH_TILES;
}

class Arena : public SceneNode {
private:
	sf::VertexArray m_vertices{sf::Quads,ARENA_TILE_COUNT*4};
public:
	Arena() {
		for (size_t x = 0; x < ARENA_WIDTH_TILES; x++)
			for (size_t y = 0; y < ARENA_HEIGHT_TILES; y++) {
				const auto index = tile_number(x,y)*4;
				Vertex * quad = &m_vertices[index];
				quad[0].position = tile_location(x,y);
				quad[1].position = tile_location(x+1,y);
				quad[2].position = tile_location(x+1,y+1);
				quad[3].position = tile_location(x,y+1);
				quad[0].color = {100,1,100};
				quad[1].color = {255,255,255};
				quad[2].color = {100,1,100};
				quad[3].color = {255,255,255};
			}
	}
protected:
	void draw_node(sf::RenderTarget& target, sf::RenderStates state) const override {
		// TODO 100 define arena concept further
		target.draw(m_vertices,state);
	}
};

SneakyWorld::SneakyWorld() : m_scene_graph(), m_arena(new Arena()) {
	m_scene_graph.attach(SceneNode::u_ptr(m_arena));
}

SneakyGame::SneakyGame() : Game(800,600,"Sneaky"), m_view(window().getDefaultView()) {}

void SneakyGame::run() {
	Game::run();
}
void SneakyGame::update(codespear::FrameTime step) {
	m_view.move(1,1);
}
void SneakyGame::draw(sf::RenderTarget &window) {
	window.setView(m_view);
	m_world.scene().draw(window,m_rstate);
}
}
