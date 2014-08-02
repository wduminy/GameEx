#include <boost/test/unit_test.hpp>
#include "../physics.h"
#include "../log.h"
using namespace codespear;
namespace {

struct PhysicsFixture {
	PhysicsWorld w;
	b2Fixture * collided_a = nullptr;
	b2Fixture * collided_b = nullptr;
	void wait_for_collision() {
		bool hit = false;
		w.set_handler([&](b2Contact * c) {
			hit = true;
			collided_a = c->GetFixtureA();
			collided_b = c->GetFixtureB();
		});
		while (!hit) w.update();
	}

};
BOOST_AUTO_TEST_SUITE( PhysicsTest )

BOOST_FIXTURE_TEST_CASE( ball_hits_rectangle, PhysicsFixture) {
	const auto size = 1.f;
	const auto bottom = 8.f;
	w.add_static_rect({0.f,bottom},10.f,size);
	auto k = w.add_dyna_circle({5.f,5.f},size/2.f);
	k->SetLinearVelocity({0,1.f});
	wait_for_collision();
	BOOST_REQUIRE((bottom - k->GetPosition().y) >= size); // bounced back
}

BOOST_FIXTURE_TEST_CASE( ball_hits_rect_chain, PhysicsFixture) {
	const auto size = 1.f;
	const auto bottom = 8.f;
	w.add_chain_rect({0.f,0.f},{10.f,bottom});
	auto k = w.add_dyna_circle({5.f,5.f},size/2.f);
	k->SetLinearVelocity({0,1.f});
	wait_for_collision();
	LOG << k->GetPosition().y;
	BOOST_REQUIRE((bottom - k->GetPosition().y) >= size/2.f); // bounced back wrt radius
}

BOOST_FIXTURE_TEST_CASE(ball_hits_static_ball, PhysicsFixture) {
	auto a = w.add_dyna_circle({1.f,1.f},1.f);
	a->SetLinearVelocity({0,1});
	auto b = w.add_static_circle({1,10},1);
	wait_for_collision();
	BOOST_REQUIRE_EQUAL(collided_a,a->GetFixtureList());
	BOOST_REQUIRE_EQUAL(collided_b,b);
}

BOOST_FIXTURE_TEST_CASE(ball_hits_dynamic_ball, PhysicsFixture) {
	auto a = w.add_dyna_circle({1.f,1.f},1.f);
	a->SetLinearVelocity({0,1});
	auto b = w.add_dyna_circle({1,10},1);
	wait_for_collision();
	BOOST_REQUIRE_EQUAL(collided_a,a->GetFixtureList());
	BOOST_REQUIRE_EQUAL(collided_b,b->GetFixtureList());
}


BOOST_AUTO_TEST_SUITE_END()
}
