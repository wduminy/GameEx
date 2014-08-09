// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include "physics.h"
#include "log.h"
#include "exception.h"

namespace codespear {

PhysicsWorld::PhysicsWorld() {
	m_world.SetContactListener(this);
	b2BodyDef def;
	def.position.Set(0.f,0.f);
	def.type = b2_staticBody;
	m_static = m_world.CreateBody(&def);
}
void PhysicsWorld::BeginContact(b2Contact* contact) {
	if (m_handler)
		m_handler(contact);
}

b2Fixture * PhysicsWorld::add_static_rect(const MeterVector& center,
		const float w, const float h) {
	b2PolygonShape shape;
	shape.SetAsBox(w/2.f,h/2.f,center,0.f);
	return m_static->CreateFixture(&shape,0.f);
}

b2Fixture * PhysicsWorld::add_static_circle(const MeterVector& center,
		const float r) {
	b2CircleShape shape;
	shape.m_p = center;
	shape.m_radius = r;
	return m_static->CreateFixture(&shape,0.f);
}

b2Fixture * PhysicsWorld::add_chain_rect(const MeterVector& lt, const MeterVector& dim) {
	b2Vec2 v[4];
	auto x = lt.x;
	auto y = lt.y;
	auto w = dim.x;
	auto h = dim.y;
	v[0].Set(x,y);
	v[1].Set(x+w,y);
	v[2].Set(x+w,y+h);
	v[3].Set(x,y+h);
	b2ChainShape shape;
	shape.CreateLoop(v,4);
	return m_static->CreateFixture(&shape,0.f);
}


b2Body * PhysicsWorld::add_dyna_circle(const MeterVector& center, float r) {
	b2BodyDef def;
	def.position = center;
	def.type = b2_dynamicBody;
	def.fixedRotation = false;
	auto * bod = m_world.CreateBody(&def);
	b2CircleShape shape;
	shape.m_p.Set(0,0);
	shape.m_radius = r;
	b2FixtureDef fix_def;
	fix_def.shape = &shape;
	fix_def.density = 1.0f;
	fix_def.friction = 0.0f;
	bod->CreateFixture(&fix_def);
	return bod;
}

const float time_step = 1.f/60.f;
const int vel_iters = 6;
const int pos_iters = 3;

void PhysicsWorld::update() {
	m_world.Step(time_step,vel_iters,pos_iters);
//	LOG << m_world.GetContactCount() << " contacts";
}

struct WorldDrawer : public b2Draw {
float scale {1.f};
sf::RenderStates states;
sf::RenderTarget * t {nullptr};
WorldDrawer() {
	AppendFlags(e_shapeBit
//			| e_jointBit
//			| e_aabbBit
//			| 	e_pairBit
//			| 	e_centerOfMassBit
			);
}
void DrawPolygon(const b2Vec2* vertices,
		int32 vertexCount, const b2Color& color) override {
	DrawSolidPolygon(vertices,vertexCount,color);
};

void DrawSolidPolygon(const b2Vec2* vertices,
		int32 vertexCount, const b2Color& color) override {
	sf::VertexArray a(sf::LinesStrip, vertexCount+1);
	for (int32 i=0; i < vertexCount;i++) {
		a[i].position = sf::Vector2f(vertices[i].x*scale, vertices[i].y*scale);
		a[i].color = sf::Color::White;
	}
	a[vertexCount] = a[0];
	t->draw(a,states);
};

void DrawCircle(const b2Vec2& center,
		float32 radius, const b2Color& color) override {

	sf::CircleShape s;
	s.setPosition({center.x*scale,center.y*scale});
	s.setOutlineColor(sf::Color::Green);
    s.setOrigin({radius*scale,radius*scale});
	s.setRadius(radius*scale);
	s.setFillColor({0,0,0,0});
	s.setOutlineThickness(2);
	t->draw(s,states);
};

void DrawSolidCircle(const b2Vec2& center,
		float32 radius, const b2Vec2& axis, const b2Color& color) override {
	DrawCircle(center,radius,color);
};

void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override {
	sf::VertexArray a(sf::Lines, 2);
	a[0].position = sf::Vector2f(p1.x*scale, p1.y*scale);
	a[1].position = sf::Vector2f(p2.x*scale, p2.x*scale);
	a[0].color = sf::Color::Red;
	a[1].color = sf::Color::Red;
	t->draw(a,states);
};

void DrawTransform(const b2Transform& xf) {
	sf::Transform t;
	t.rotate(xf.q.GetAngle()/(3.14f*2.f));
	t.translate(xf.p.x*scale,xf.p.y*scale);
	states.transform *= t;
};

} drawer;

void PhysicsWorld::debug_draw(sf::RenderTarget &t, float pixels_per_meter,
		sf::RenderStates states) {
	drawer.scale = pixels_per_meter;
	drawer.states = states;
	drawer.t = &t;
	drawer.states.blendMode  = sf::BlendAdd;
	m_world.SetDebugDraw(&drawer);
	m_world.DrawDebugData();
}

void Physicalb2Body::assign_body(b2Body * b,void * data) {
	check_that(m_body == nullptr);
	m_body = b;
	m_body->GetFixtureList()->SetUserData(data);
}

void Physicalb2Fixture::assign_body(b2Fixture * b, void * data) {
	check_that(m_body == nullptr);
	m_body = b;
	m_body->SetUserData(data);
}

void Physicalb2Fixture::destroy_body() {
	check_that(m_body);
	m_body->GetBody()->DestroyFixture(m_body);
	m_body = nullptr;
}


void PhysicsWorldWithBodies::handle_collision(b2Contact * contact) {
	auto a = contact->GetFixtureA()->GetUserData();
	auto b = contact->GetFixtureB()->GetUserData();
	// we order a and b
	Body * b1 = nullptr;
	Body * b2 = nullptr;
	if (a && b) {
		b1 = reinterpret_cast<Body *>(a);
		b2 = reinterpret_cast<Body *>(b);
		if (b1->type > b2->type) {
			Body * t = b1;
			b1 = b2;
			b2 = t;
		}
	} else
		b2 = reinterpret_cast<Body *>(a?a:b);
	if (m_body_handler)
		m_body_handler(b1,b2);
}

void PhysicsWorldWithBodies::set_handler(BodyHitHandler h) {
	PhysicsWorld::set_handler([&](b2Contact* c) {handle_collision(c);});
	m_body_handler = h;
}

}
