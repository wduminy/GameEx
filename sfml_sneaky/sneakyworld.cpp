// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include "sneakyworld.h"
#include "exception.h"
#include "flaremap.h"
#include "gui.h"
#include "physics.h"
#include "wang2edge.h"
#include "music.h"
#include <SFML/Audio.hpp>

using namespace codespear;
// TODO 900 Show keys to use on the title page
namespace sneaky {
using std::vector;
using Vector = sf::Vector2f;
using sf::Vertex;

const auto PIXELS_PER_METER = 300.f;
const auto METERS_PER_PIXEL = 1.f/PIXELS_PER_METER;

const auto ARENA_SQUARE = 100.f;  // pixel size of one square
const auto ARENA_SQUARE_M = ARENA_SQUARE * METERS_PER_PIXEL;
const auto HEAD_SIZE = ARENA_SQUARE / 2.f; // pixel size of head
const auto HEAD_RADIUS_M = HEAD_SIZE * METERS_PER_PIXEL * 0.5f;
const size_t ARENA_WIDTH_TILES = 10;
const size_t ARENA_HEIGHT_TILES = 10;
const auto ARENA_WIDTH_M = ARENA_WIDTH_TILES * ARENA_SQUARE * METERS_PER_PIXEL;
const auto ARENA_HEIGHT_M = ARENA_HEIGHT_TILES * ARENA_SQUARE * METERS_PER_PIXEL;
const size_t ARENA_TILE_COUNT = ARENA_WIDTH_TILES * ARENA_HEIGHT_TILES;
const char* FLAREMAP_FILE_NAME = "media/sneaky/flaremap.txt";

const float RADS_IN_CIRCLE = 3.14f * 2.f;
const float DEGS_IN_CIRCLE = 360.f;
const float DEGS_IN_RAD = DEGS_IN_CIRCLE/RADS_IN_CIRCLE;
const float RADS_IN_DEG = RADS_IN_CIRCLE/DEGS_IN_CIRCLE;
const float HEAD_SPEED = .8f;
const float SCROLL_SPEED = HEAD_SPEED * 5.f;
const float SCROLL_THRESHOLD = HEAD_SIZE * 3.5f;
const float HEAD_TURN_SPEED = 320.f;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// TODO 900 Create ghosts; something to chase head
// TODO 900 Head steers with explicit forward (turns without movement?)
class Head : public SpriteNode {
private:
	float m_speed{0.f};
	float m_rotation_speed {0.f};
	b2Body * m_body;
public:
	Head(const sf::Texture & tex) :
		SpriteNode(tex,{120*3,0,90,90}, HEAD_SIZE/(90.f)) {
		m_body = nullptr;
	}

	void update() {
		setPosition(m_body->GetPosition().x * PIXELS_PER_METER,
				    m_body->GetPosition().y * PIXELS_PER_METER);
		setRotation(m_body->GetAngle()*DEGS_IN_RAD);
		adjust_velocity();
	}

	void turn_left() {m_rotation_speed = -HEAD_TURN_SPEED; }
	void turn_right() {m_rotation_speed = HEAD_TURN_SPEED; }
	void go_straight() {m_rotation_speed = 0.0f; }
	/* x and y are "square coordinates" */
	void create_body(PhysicsWorld &world, float x, float y) {
		// TODO 800 check that m_body is deleted somewhere (maybe it is destroyed with the world)
		check_that(m_body == nullptr); // cannot have two bodies for one head
		m_body = world.add_dyna_circle(
				x*ARENA_SQUARE_M+ARENA_SQUARE_M/2.f,
				y*ARENA_SQUARE_M+ARENA_SQUARE_M/2.f,HEAD_RADIUS_M);
		set_speed(HEAD_SPEED);
		update();
	}
	bool has_body() const {return m_body != nullptr;}
private:

	void set_speed(float speed) {
		m_speed = speed;
		adjust_velocity();
	}

	void adjust_velocity() {
		sf::Transform t;
		t.rotate(getRotation());
		auto velocity = t.transformPoint(0,m_speed);
		m_body->SetLinearVelocity({velocity.x, velocity.y});
		m_body->SetAngularVelocity(m_rotation_speed*RADS_IN_DEG);
	}

};
// TODO 100 Head must eat pills
class Pill : public SpriteNode {

};

class Arena : public SceneNode {
private:
	sf::VertexArray m_vertices{sf::Quads,ARENA_TILE_COUNT*4};
public:
	Arena(PhysicsWorld &world, const FlareMapLayer &field_map) {
		world.add_chain_rect(0,0,ARENA_WIDTH_M, ARENA_HEIGHT_M);
		Wang2EdgeField field(ARENA_WIDTH_TILES, ARENA_HEIGHT_TILES, field_map);
		field.fill_vertices(90.f,ARENA_SQUARE,&m_vertices[0]);
		field.fill_world(ARENA_SQUARE_M, world);
	}
protected:
	void draw_node(sf::RenderTarget& target, sf::RenderStates state) const override {
		target.draw(m_vertices,state);
	}
};

class PlayState : public State {
private:
	sf::RenderStates m_rstate;
	SceneGraph m_scene_graph;
	Arena * m_arena;
	Head * m_head;
	sf::View m_view;
	PhysicsWorld m_world;
	sf::SoundBuffer m_wall_hit_sound;
	sf::Sound m_sound;
public:
	PlayState(StateStack &stack, Context& context) : State{stack,context},
		m_view(m_context.window->getDefaultView()) {
		FlareMap fm(FLAREMAP_FILE_NAME);
		m_scene_graph.attach(m_arena = new Arena(m_world,fm.layer().at("Level1")));
		m_arena->attach(m_head = new Head(*context.texture));
		// TODO 050 get head position from flaremap data
		Wang2EdgeField field(ARENA_WIDTH_TILES, ARENA_HEIGHT_TILES, fm.layer().at("Level1_o"));
		field.visit([&](const Tile &t){
			if (t.tile == 16) // head
				m_head->create_body(m_world,t.x,t.y);
		});
		check_that(m_head->has_body()); // the map must specify a place for the body
		check_that(m_wall_hit_sound.loadFromFile("media/sneaky/wall.wav"));
		m_view.setCenter(m_head->getPosition());
		m_world.set_handler([&](b2Contact * contact){m_sound.setBuffer(m_wall_hit_sound);m_sound.play();});
	}

	void update(FrameTime step) override {
		bool left = false;
		bool right = false;
		bool up = false;
		bool down = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
			left = true;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
			right = true;
		if (left != right) {
			if (left)
				m_head->turn_left();
			if (right)
				m_head->turn_right();
		} else
			m_head->go_straight();

		// Adjust the view to keep up with the head
		auto delta = m_view.getCenter() - m_head->getPosition();
		left =  delta.x > SCROLL_THRESHOLD;
		right = delta.x < -SCROLL_THRESHOLD;
		up = delta.y > SCROLL_THRESHOLD;
		down = delta.y < -SCROLL_THRESHOLD;
		if (left)
			m_view.move(-SCROLL_SPEED,0.f);
		if (right)
			m_view.move(SCROLL_SPEED,0.f);
		if (up)
			m_view.move(0.f,-SCROLL_SPEED);
		if (down)
			m_view.move(0.f,SCROLL_SPEED);
		m_head->update();
		m_world.update();
	}
	void draw() override {
		m_context.window->setView(m_view);
		m_rstate.texture = m_context.texture;
		m_scene_graph.draw(*m_context.window,m_rstate);
//		m_world.debug_draw(*m_context.window,PIXELS_PER_METER, m_rstate);
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
		return false;
	}
	void update(FrameTime step) final {
	}
	void draw() final {
		m_context.window->draw(m_panel,states);
	}
};

class AmbientState : public State {
public:
	AmbientState(StateStack &stack, Context& context) : State{stack,context} {}
	bool handle_event(const sf::Event &e) final {
		if (e.type == sf::Event::KeyPressed
				&& e.key.code == sf::Keyboard::M)
			MusicPlayer::instance.toggle();
		return false;
	}
	void update(FrameTime step) final {}
	void draw() final {}
};

SneakyGame::SneakyGame() : Game(SCREEN_WIDTH,SCREEN_HEIGHT,"Sneaky") {}

void SneakyGame::init() {
	check_that(m_texture.loadFromFile("media/sneaky/tiles.png"));
	check_that(m_font.loadFromFile("media/sneaky/font.ttf"))
	WidgetSkin::instance.texture = &m_texture;
	WidgetSkin::instance.font = &m_font;
	const sf::Vector2i START{120*3,30*3}, SIZE{44*3,21*3}, GAP{0,22*3};
	WidgetSkin::instance.button_normal = {START,SIZE};
	WidgetSkin::instance.button_selected = {START+GAP,SIZE};
	WidgetSkin::instance.button_pressed = {START+GAP*2,SIZE};
	WidgetSkin::instance.button_scale = 5.f/3.f;
	m_context.texture = &m_texture;
	m_stack.register_state<TitleState>(GameState::Title);
	m_stack.register_state<PlayState>(GameState::Play);
	m_stack.register_state<AmbientState>(GameState::Ambient);
	m_stack.push(GameState::Ambient);
	MusicPlayer::instance.play("media/sneaky/music.ogg");
}

}
