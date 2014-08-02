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
#include "command.h"
#include <SFML/Audio.hpp>

using namespace codespear;
// TODO 900 Show keys to use on the title page
namespace sneaky {
using std::vector;
using Vector = sf::Vector2f;
using sf::Vertex;
/** ScreenVector is size in screen pixels */
using PixelVector = sf::Vector2f;
/** TileVector is the position of the tile */
using TileVector = sf::Vector2f;

using Pixels = float;
using Meters = float;
using Tiles = float;

const size_t ARENA_WIDTH_TILES = 10;
const size_t ARENA_HEIGHT_TILES = 10;
const size_t SCREEN_WIDTH = 800;
const size_t SCREEN_HEIGHT = 600;

const Pixels PIXELS_PER_METER = 100.f;
const Pixels PIXELS_PER_TILE = 120.f;
const Meters METERS_PER_PIXEL = 1.f/PIXELS_PER_METER;
const Tiles TILES_PER_METER = PIXELS_PER_METER / PIXELS_PER_TILE;
const Meters METERS_PER_TILE = 1.f/TILES_PER_METER;
const Meters METERS_PER_HALF_TILE = METERS_PER_TILE / 2.0f;

MeterVector to_real(const TileVector &v) {return MeterVector{v.x*METERS_PER_TILE,v.y*METERS_PER_TILE};}
MeterVector to_real_center(const TileVector &v) {
	return MeterVector{v.x*METERS_PER_TILE + METERS_PER_HALF_TILE,
		               v.y*METERS_PER_TILE + METERS_PER_HALF_TILE};
}
MeterVector to_real_center(const int x, const int y) {return to_real_center({x*1.f,y*1.f});}
PixelVector to_screen(const MeterVector &v) {return PixelVector{v.x * PIXELS_PER_METER,v.y * PIXELS_PER_METER};}

const Pixels HEAD_SIZE = PIXELS_PER_TILE / 2.f; // pixel size of head
const Meters HEAD_RADIUS_M = HEAD_SIZE * METERS_PER_PIXEL * 0.5f;

const TileVector TILE_RB{ARENA_WIDTH_TILES,ARENA_HEIGHT_TILES};
const MeterVector REAL_RB = to_real(TILE_RB);
const PixelVector PIXEL_RB(SCREEN_WIDTH,SCREEN_HEIGHT);

const size_t ARENA_TILE_COUNT = ARENA_WIDTH_TILES * ARENA_HEIGHT_TILES;
const char* FLAREMAP_FILE_NAME = "media/sneaky/flaremap.txt";
using Radains = float;
using Degrees = float;

const Radains RADS_IN_CIRCLE = 3.14f * 2.f;
const Degrees DEGS_IN_CIRCLE = 360.f;
const Degrees DEGS_IN_RAD = DEGS_IN_CIRCLE/RADS_IN_CIRCLE;
const Radains RADS_IN_DEG = RADS_IN_CIRCLE/DEGS_IN_CIRCLE;
const Pixels SCROLL_THRESHOLD = SCREEN_HEIGHT * 0.35; // deviation from centre of screen

using MetersPerSecond = float;
using DegreesPerSecond = float;
using PixelsPerMilliSecond = float;
const DegreesPerSecond HEAD_TURN_SPEED = 320.f;
const MetersPerSecond HEAD_SPEED = METERS_PER_TILE * 2.5;
const PixelsPerMilliSecond SCROLL_SPEED = HEAD_SPEED * PIXELS_PER_METER / 1000.f;

// TODO 900 Create ghosts; something to chase head
// TODO 900 Head steers with explicit forward (turns without movement?)
class Head : public SpriteNode {
private:
	MetersPerSecond m_speed{0.f};
	DegreesPerSecond m_rotation_speed {0.f};
	b2Body * m_body;
public:
	Head(const sf::Texture & tex) :
		SpriteNode(tex,{120*3,0,90,90}, HEAD_SIZE/(90.f)) {
		m_body = nullptr;
	}

	void update() {
		const auto left = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left));
		const auto right = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right));
		if (left != right) {
			if (left)
				m_rotation_speed = -HEAD_TURN_SPEED;
			if (right)
				m_rotation_speed = HEAD_TURN_SPEED;
		} else
			m_rotation_speed = 0.0f;
		setPosition(to_screen(m_body->GetPosition()));
		setRotation(m_body->GetAngle()*DEGS_IN_RAD);
		adjust_velocity();
	}

	/* x and y are "square coordinates" */
	void create_body(PhysicsWorld &world, const TileVector &v) {
		check_that(m_body == nullptr); // cannot have two bodies for one head
		m_body = world.add_dyna_circle(to_real_center(v),HEAD_RADIUS_M);
		set_speed(HEAD_SPEED);
		update();
	}
	bool has_body() const {return m_body != nullptr;}
private:

	void set_speed(MetersPerSecond speed) {
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
class Pill : public SpriteNode {
private:
	b2Fixture * m_body{nullptr};
public:
	Pill(const sf::Texture & tex, PhysicsWorld &world, const MeterVector &position) :
		SpriteNode(tex,{120*3,285,30,30}, HEAD_SIZE/(90.f))  {
		setPosition(to_screen(position));
		m_body = world.add_static_circle(position, HEAD_RADIUS_M/3.f);
		m_body->SetUserData(this);
	}

	void destroy() {
		m_body->GetBody()->DestroyFixture(m_body);
		m_body = nullptr;
	}
};

class Arena : public SceneNode {
private:
	sf::VertexArray m_vertices{sf::Quads,ARENA_TILE_COUNT*4};
public:
	Arena(PhysicsWorld &world, const FlareMapLayer &field_map) {
		world.add_chain_rect({0,0},REAL_RB);
		Wang2EdgeField field(ARENA_WIDTH_TILES, ARENA_HEIGHT_TILES, field_map);
		field.fill_vertices(90.f,PIXELS_PER_TILE,&m_vertices[0]);
		field.fill_world(METERS_PER_TILE, world);
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
	CommandQueue m_commands;
public:
	PlayState(StateStack &stack, Context& context) : State{stack,context},
		m_view(m_context.window->getDefaultView()) {
		FlareMap fm(FLAREMAP_FILE_NAME);
		m_scene_graph.attach(m_arena = new Arena(m_world,fm.layer().at("Level1")));
		m_arena->attach(m_head = new Head(*context.texture));
		// Add the pills and move the head based on the flare map data
		Wang2EdgeField field(ARENA_WIDTH_TILES, ARENA_HEIGHT_TILES, fm.layer().at("Level1_o"));
		field.visit([&](const Tile &t){
			if (t.tile == 16) // head
				m_head->create_body(m_world,{static_cast<float>(t.x),static_cast<float>(t.y)});
			else if (t.tile == 17) // pill
				m_scene_graph.attach(new Pill(*context.texture,m_world,to_real_center(t.x,t.y)));
		});
		check_that(m_head->has_body()); // the map must specify a place for the body
		check_that(m_wall_hit_sound.loadFromFile("media/sneaky/wall.wav"));
		m_view.setCenter(m_head->getPosition());
		m_world.set_handler([&](b2Contact * contact){
			auto a = contact->GetFixtureA()->GetUserData();
			auto b = contact->GetFixtureB()->GetUserData();
			if (a || b) {
				// we hit something that is not a wall;
				// it must be a pill
				auto p = reinterpret_cast<Pill *> (a?a:b);
				m_commands.schedule([=](Milliseconds t){
					p->destroy();
					m_scene_graph.detach(p);
				});
			}
			m_sound.setBuffer(m_wall_hit_sound);m_sound.play();
		});
	}

	void update(Milliseconds step) override {
		m_head->update();
		// Adjust the view to keep up with the head
		auto delta = m_view.getCenter() - m_head->getPosition();
		const bool left =  delta.x > SCROLL_THRESHOLD;
		const bool right = delta.x < -SCROLL_THRESHOLD;
		const bool up = delta.y > SCROLL_THRESHOLD;
		const bool down = delta.y < -SCROLL_THRESHOLD;
		if (left)
			m_view.move(-SCROLL_SPEED * step,0.f);
		if (right)
			m_view.move(SCROLL_SPEED * step,0.f);
		if (up)
			m_view.move(0.f,-SCROLL_SPEED * step);
		if (down)
			m_view.move(0.f,SCROLL_SPEED * step);
		m_world.update();
		m_commands.update(step);
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
	void update(Milliseconds step) final {
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
	void update(Milliseconds step) final {}
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
