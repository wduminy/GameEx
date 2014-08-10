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
#include "navmap.h"
#include <SFML/Audio.hpp>

using namespace codespear;
// TODO 900 Show keys to use on the title page
// TODO 900 Implement pause
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

using codespear::body_t;
enum class body_t{Head,Pill,Ghost};

// TODO 900 Head steers with explicit forward (turns without movement?)
class Head : public SpriteNode, public Physicalb2Body{
private:
	MetersPerSecond m_speed{0.f};
	DegreesPerSecond m_rotation_speed {0.f};
public:
	Head(const sf::Texture & tex) :
		SpriteNode(tex,{120*3,0,90,90}, HEAD_SIZE/(90.f)),
		Physicalb2Body({body_t::Head}){
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
		assign_body(world.add_dyna_circle(to_real_center(v),HEAD_RADIUS_M),this);
		set_speed(HEAD_SPEED);
		update();
	}
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

//TODO 100 let the ghosts plan their path using the nav map
class Ghost : public SpriteNode, public Physicalb2Fixture {
public:
	Ghost(const sf::Texture & tex, PhysicsWorld &world, const MeterVector &position) :
		SpriteNode(tex,{120*3+30,285,30,30}, HEAD_SIZE/(30.f)),
		Physicalb2Fixture(body_t::Ghost){
		assign_body(world.add_static_circle(position, HEAD_RADIUS_M),this);
		setPosition(to_screen(position));
	}
};

class Pill : public SpriteNode, public Physicalb2Fixture {
public:
	Pill(const sf::Texture & tex, PhysicsWorld &world, const MeterVector &position) :
		SpriteNode(tex,{120*3,285,30,30}, HEAD_SIZE/(30.f)),
		Physicalb2Fixture(body_t::Pill){
		assign_body(world.add_static_circle(position, HEAD_RADIUS_M),this);
		setPosition(to_screen(position));
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

class Hud : public SceneNode {
	mutable Panel m_panel;
	Label * m_label;
	const sf::View &m_view;
	const size_t m_total_pills;
	size_t m_pills_eaten{0};
public:
	Hud(const sf::View &view, size_t pill_count) :
		m_view(view),
		m_total_pills(pill_count) {
		m_panel += (m_label = new Label(""));
		update_text();
	}
	void eat_pill() {
		m_pills_eaten++;
		update_text();
	}

protected:
	void update_text() {
		std::ostringstream ss;
		ss << "You got " << m_pills_eaten << " out of " << m_total_pills;
		m_label->set_text(ss.str());
	}
	void draw_node(sf::RenderTarget& target, sf::RenderStates state) const final {
		m_panel.setPosition(m_view.getCenter() - m_view.getSize()/2.2f);
		m_panel.draw(target,state);
	}
};

class PlayState : public State {
private:
	sf::RenderStates m_rstate;
	SceneGraph m_scene_graph;
	Arena * m_arena;
	Head * m_head;
	sf::View m_view;
	PhysicsWorldWithBodies m_world;
	sf::SoundBuffer m_wall_hit_sound;
	sf::Sound m_sound;
	CommandQueue m_commands;
	Hud * m_hud;
	NavigationMap m_navmap;
public:
	PlayState(StateStack &stack, Context& context) : State{stack,context},
		m_view(m_context.window->getDefaultView()) {
		FlareMap fm(FLAREMAP_FILE_NAME);
		m_scene_graph.attach(m_arena = new Arena(m_world,fm.layer().at("Level1")));
		m_arena->attach(m_head = new Head(*context.texture));
		// Add the pills and move the head based on the flare map data
		size_t pill_count = 0;
		size_t ghost_count = 0;
		Wang2EdgeField field(ARENA_WIDTH_TILES, ARENA_HEIGHT_TILES, fm.layer().at("Level1_o"));

		field.visit([&](const Tile &t){
			// TODO 090 fill the nav map
			auto const rc = to_real_center(t.x,t.y);
			m_navmap.add_relative(PathSegment{rc,RIGHT},true);
			m_navmap.add_relative(PathSegment{rc,DOWN},true);
			if (t.tile == 16) // head
				m_head->create_body(m_world,{static_cast<float>(t.x),static_cast<float>(t.y)});
			else if (t.tile == 17) { // pill
				auto p = new Pill(*context.texture,m_world,rc);
				m_scene_graph.attach(p);
				pill_count++;
			} else if (t.tile == 18) { // ghost
				auto g = new Ghost(*context.texture,m_world,rc);
				m_scene_graph.attach(g);
				ghost_count++;
			}
		});
		check_that(m_head->has_body()); // the map must specify a place for the body
		check_that(ghost_count > 0);
		m_scene_graph.attach(m_hud = new Hud(m_view,pill_count));
		check_that(m_wall_hit_sound.loadFromFile("media/sneaky/wall.wav"));
		m_view.setCenter(m_head->getPosition());
		m_world.set_handler([&](Body *a, Body *b){handle_collision(a,b);});
	}

	void handle_collision(Body * a, Body * b) {
		if (a) {
			// we hit something that is not a wall;
			// it must be a pill
			// TODO 900 handle collisions with ghosts
			if (b->type == body_t::Pill) {
				m_hud->eat_pill();
				auto p = static_cast<Pill *> (b);
				m_commands.schedule([=](Milliseconds t){
					p->destroy_body();
					m_scene_graph.detach(p);
				});
			}
		}
		m_sound.setBuffer(m_wall_hit_sound);m_sound.play();
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
