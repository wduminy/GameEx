#include <boost/test/unit_test.hpp>
#include "../physics.h"
#include "../log.h"
using namespace codespear;
namespace {
BOOST_AUTO_TEST_SUITE( PhysicsTest )

BOOST_AUTO_TEST_CASE( ball_hits_rectangle ) {
	PhysicsWorld w;
	const auto size = 1.f;
	const auto bottom = 8.f;
	w.add_static_rect(0.f,bottom,10.f,size);
	auto k = w.add_dyna_circle(5.f,5.f,size/2.f);
	k->SetLinearVelocity({0,1.f});
	bool hit = false;
	w.set_handler([&](b2Contact * c) {
		hit = true;
		BOOST_REQUIRE(bottom - k->GetPosition().y < size);
	});
	while (!hit) {
		w.update();
		BOOST_REQUIRE(k->GetPosition().y < 11.f);
	}
	BOOST_REQUIRE((bottom - k->GetPosition().y) >= size); // bounced back
}

BOOST_AUTO_TEST_CASE( ball_hits_rect_chain ) {
	PhysicsWorld w;
	const auto size = 1.f;
	const auto bottom = 8.f;
	w.add_chain_rect(0.f,0.f,10.f,bottom);
	auto k = w.add_dyna_circle(5.f,5.f,size/2.f);
	k->SetLinearVelocity({0,1.f});
	bool hit = false;
	w.set_handler([&](b2Contact * c) {
		hit = true;
		BOOST_REQUIRE(bottom - k->GetPosition().y < size);
	});
	while (!hit) {
		w.update();
		BOOST_REQUIRE(k->GetPosition().y < 11.f);
	}
	LOG << k->GetPosition().y;
	BOOST_REQUIRE((bottom - k->GetPosition().y) >= size/2.f); // bounced back wrt radius
}


BOOST_AUTO_TEST_SUITE_END()
}
