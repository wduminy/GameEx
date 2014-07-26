#include <boost/test/unit_test.hpp>
#include "../physics.h"
#include "../log.h"
using namespace codespear;
namespace {
BOOST_AUTO_TEST_SUITE( PhysicsTest )

BOOST_AUTO_TEST_CASE( ball_hits_rectangle ) {
	PhysicsWorld w;
	const auto size = 1.f;
	auto b = w.add_static_rect(0.f,8.f,10.f,size);
	auto k = w.add_kine_circle(5.f,5.f,size/2.f);
	k->SetLinearVelocity({0,1.f});
	bool hit = false;
	w.set_handler([&](b2Contact * c) {
		hit = true;
		BOOST_REQUIRE(b->GetPosition().y - k->GetPosition().y < size);
	});
	while (!hit) {
//		TRACE << k->GetPosition().y << " " << b->GetPosition().y << " "
//		  << (b->GetPosition().y - k->GetPosition().y);
		w.update();
		BOOST_REQUIRE(k->GetPosition().y < 11.f);
	}
	BOOST_REQUIRE((b->GetPosition().y - k->GetPosition().y) >= size); // bounced back
}


BOOST_AUTO_TEST_SUITE_END()
}
