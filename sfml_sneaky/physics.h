// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#pragma once
#include <Box2D/Box2D.h>
#include <functional>

namespace codespear {

using HitHandler = std::function<void (b2Contact *)>;

class PhysicsWorld : public b2ContactListener {
private:
	b2World m_world{{0.f,0.f}};
	HitHandler m_handler;
	b2Body * m_static;
public:

	PhysicsWorld();
	b2Fixture * add_static_rect(const float x, const float y, const float w, const float h);
	b2Fixture * add_chain_rect(const float x, const float y, const float w, const float h);
	b2Body * add_dyna_circle(float x, float y, float r);
	void set_handler(HitHandler h)  {m_handler = h;}
	void update();
	void BeginContact(b2Contact* contact) override;
};

}
