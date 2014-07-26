// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include "sneakyworld.h"
#include "exception.h"
#include "flaremap.h"
#include "gui.h"

using namespace codespear;

namespace sneaky {
using std::vector;
using Point = sf::Vector2f;
using sf::Vertex;

const auto ARENA_SQUARE = 50.f;
const size_t ARENA_WIDTH_TILES = 10;
const size_t ARENA_HEIGHT_TILES = 10;
const size_t ARENA_TILE_COUNT = ARENA_WIDTH_TILES * ARENA_HEIGHT_TILES;
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

class Head : public SpriteNode {
public:
	Head(const sf::Texture & tex) : SpriteNode(tex,{120,0,10,10}, ARENA_SQUARE/30.f) {}

};

class Arena : public SceneNode {
private:
	sf::VertexArray m_vertices{sf::Quads,ARENA_TILE_COUNT*4};
public:
	Arena() {
		FlareMap fm(FLAREMAP_FILE_NAME);
		auto ixs = fm.layer().at("Level1");
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
		target.draw(m_vertices,state);
	}
};

SneakyWorld::SneakyWorld() : m_scene_graph(), m_arena(nullptr) , m_head(nullptr) {
}

void SneakyWorld::update(FrameTime step) {
	m_head->move(1,1);
	m_head->rotate(1.f);
}


void SneakyWorld::init(sf::Texture& texture) {
	m_scene_graph.attach(m_arena = new Arena());
	m_arena->attach(m_head = new Head(texture));
}

class PlayState : public State {
private:
	sf::RenderStates m_rstate;
	SneakyWorld m_world;
	sf::View m_view;
public:
	PlayState(StateStack &stack, Context& context) : State{stack,context},
		m_view(m_context.window->getDefaultView()) {
		m_world.init(*context.texture);
	}
	void update(FrameTime step) override {
		m_world.update(step);
	}
	void draw() override {
		m_context.window->setView(m_view);
		m_rstate.texture = m_context.texture;
		m_world.scene().draw(*m_context.window,m_rstate);
	}
};


class TitleState : public State {
private:
	Panel m_panel;
	sf::RenderStates states;
public:
	TitleState(StateStack &stack, Context &context) : State{stack,context} {
		m_panel += new Button{"Play",{100, 10},[&]{switch_to(GameState::Play);}};
		m_panel += new Button{"Credits",{100, 110},[]{}};
		m_panel += new Button{"Quit",  {100,210},[&](){pop();}};
	}
	bool handle_event(const sf::Event &e) final {
		m_panel.handle_event(e);
		return true;
	}
	void update(FrameTime step) final {
	}
	void draw() final {
		m_context.window->draw(m_panel,states);
	}
};

SneakyGame::SneakyGame() : Game(800,600,"Sneaky",GameState::Title) {}

void SneakyGame::init() {
	check_that(m_texture.loadFromFile("media/sneaky/tiles.png"));
	check_that(m_font.loadFromFile("media/sneaky/font.ttf"))
	WidgetSkin::instance.texture = &m_texture;
	WidgetSkin::instance.font = &m_font;
	const sf::Vector2i START{120,30}, SIZE{44,21}, GAP{0,22};
	WidgetSkin::instance.button_normal = {START,SIZE};
	WidgetSkin::instance.button_selected = {START+GAP,SIZE};
	WidgetSkin::instance.button_pressed = {START+GAP*2,SIZE};
	WidgetSkin::instance.button_scale = 5.f;
	m_context.texture = &m_texture;
	m_stack.register_state<TitleState>(GameState::Title);
	m_stack.register_state<PlayState>(GameState::Play);

}

}
