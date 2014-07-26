// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include "physics.h"
#include "log.h"

namespace codespear {

PhysicsWorld::PhysicsWorld() {
	m_world.SetContactListener(this);
}
void PhysicsWorld::BeginContact(b2Contact* contact) {
	if (m_handler)
		m_handler(contact);
}

b2Body * PhysicsWorld::add_static_rect(const float x, const float y,
		const float w, const float h) {
	b2BodyDef def;
	def.position.Set(x + w/2.f,y + h/2.f);
	def.type = b2_staticBody;
	b2Body * bod = m_world.CreateBody(&def);
	b2PolygonShape shape;
	shape.SetAsBox(w/2.f,h/2.f);
	bod->CreateFixture(&shape,0.f);
	return bod;
}

b2Body * PhysicsWorld::add_kine_circle(float x, float y, float r) {
	b2BodyDef def;
	def.position.Set(x,y);
	def.type = b2_dynamicBody;
	auto * bod = m_world.CreateBody(&def);
	b2CircleShape shape;
	shape.m_p.Set(0,0);
	shape.m_radius = r;
	b2FixtureDef fix_def;
	fix_def.shape = &shape;
	fix_def.density = 1.0f;
	fix_def.friction = 0.3f;
	bod->CreateFixture(&fix_def);
	return bod;
}

const float time_step = 1.f/60.f;
const int vel_iters = 6;
const int pos_iters = 2;

void PhysicsWorld::update() {
	m_world.Step(time_step,vel_iters,pos_iters);
//	LOG << m_world.GetContactCount() << " contacts";
}



}
