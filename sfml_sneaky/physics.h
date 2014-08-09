// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#pragma once
#include <Box2D/Box2D.h>
#include <functional>
#include <SFML/Graphics.hpp>

namespace codespear {

/*
 * RealVector is  position in the physical world: 1.0f equals 1 meter
 */
using MeterVector = b2Vec2;
enum class body_t; // to be defined by specific game

class Body {
	public:
		 const body_t type;
};

using HitHandler = std::function<void (b2Contact *)>;
using BodyHitHandler = std::function<void (Body *a, Body *b)>;

template <class bodyT>
class PhysicalBody : public Body {
	protected:
		bodyT * m_body = nullptr;
		virtual void assign_body(bodyT * b, void * data) = 0;
	public:
		PhysicalBody<bodyT>(body_t t) : Body{t} {};
		bool has_body() const {return m_body != nullptr;}
		virtual ~PhysicalBody() {}
};

class Physicalb2Body : public PhysicalBody<b2Body> {
	protected:
		void assign_body(b2Body * b, void * data) override;
	public:
		Physicalb2Body(body_t t) : PhysicalBody<b2Body>{t} {}
};

class Physicalb2Fixture : public PhysicalBody<b2Fixture> {
	protected:
		void assign_body(b2Fixture * b, void * data) override;
	public:
		Physicalb2Fixture(body_t t) : PhysicalBody<b2Fixture>(t) {}
		void destroy_body();
};

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

class PhysicsWorldWithBodies : public PhysicsWorld {
private:
	BodyHitHandler m_body_handler;
	void handle_collision(b2Contact * c);
public:
	void set_handler(BodyHitHandler h);
};

}
