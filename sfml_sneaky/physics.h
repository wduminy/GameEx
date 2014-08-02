// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#pragma once
#include <Box2D/Box2D.h>
#include <functional>
#include <SFML/Graphics.hpp>

namespace codespear {

using HitHandler = std::function<void (b2Contact *)>;

/*
 * RealVector is  position in the physical world: 1.0f equals 1 meter
 */
using MeterVector = b2Vec2;

/*
 * The add_* methods create new objects and allows you to
 * reference the objects, but you should not delete them.
 * Their lifetime is tied to the PhysicsWorld instance
 */
class PhysicsWorld : public b2ContactListener {
private:
	b2World m_world{{0.f,0.f}};
	HitHandler m_handler;
	b2Body * m_static;
public:
	PhysicsWorld();
	b2Fixture * add_static_rect(const MeterVector& center, const float w, const float h);
	b2Fixture * add_chain_rect(const MeterVector& left_top, const MeterVector& dim);
	b2Fixture * add_static_circle(const MeterVector& center, float r);
	b2Body * add_dyna_circle(const MeterVector& center, float r);
	void set_handler(HitHandler h)  {m_handler = h;}
	void update();
	void BeginContact(b2Contact* contact) override;
	void debug_draw(sf::RenderTarget &t, float pixels_per_meter, sf::RenderStates states);
private:
	PhysicsWorld(const PhysicsWorld &other); // hide copy constructor
};

}
