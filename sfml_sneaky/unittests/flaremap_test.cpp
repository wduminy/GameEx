#include <boost/test/unit_test.hpp>
#include "../flaremap.h"
using namespace codespear;
namespace {
BOOST_AUTO_TEST_SUITE( FlareMapTests )

BOOST_AUTO_TEST_CASE( read_must_work ) {
	FlareMap fm("media/test/flaremap.txt");
	BOOST_REQUIRE(fm.layer().size() == 2);
	BOOST_REQUIRE_EQUAL(fm.layer().at("Tile Layer 1")[0],2);
	BOOST_REQUIRE_EQUAL(fm.layer().at("Tile Layer 1").size(),100);
	BOOST_REQUIRE_EQUAL(fm.layer().at("Tile Layer 1").at(99),12);
	BOOST_REQUIRE_EQUAL(fm.layer().at("Layer 2").at(99),12);
}

BOOST_AUTO_TEST_CASE(file_missing) {
	BOOST_CHECK_THROW(FlareMap("Badfilename"),std::runtime_error);
}
BOOST_AUTO_TEST_SUITE_END()
}
