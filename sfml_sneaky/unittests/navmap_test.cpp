#include <boost/test/unit_test.hpp>
#include "../navmap.h"
#include <algorithm>

using namespace codespear;
using namespace boost::test_tools;

namespace {



BOOST_AUTO_TEST_SUITE( NavMapTests )

struct NavFixture {
	NavigationMap map;
	void add_all_dirs(const MeterVector & v) {
		map.add_relative({v, DOWN});
		map.add_relative({v, UP});
		map.add_relative({v, LEFT});
		map.add_relative({v, RIGHT});
	}

	predicate_result has_n_from(const MeterVector &v, size_t n)
	{
		auto fs = map.from(v);
		if (fs.size() != n) {
			predicate_result res( false );
			res.message() << "Expected from count " << n
					<< " but found " << fs.size()
					<< " for " << v
					<< ". Actual points are: " << fs;
			return res;
		} else
			return true;
	}

	predicate_result has_path(const MeterVector &a, const MeterVector &b)
	{
		auto fs = map.from(a);
		auto fnd = std::find(fs.begin(), fs.end(),b);
		if (fnd == fs.end()) {
			predicate_result res( false );
			res.message() << "Expected path from " << a
					<< " to " << b
					<< ". Actual paths found: " << fs;
			return res;
		} else
			return true;
	}

};

#define check_from(V,N) BOOST_CHECK(has_n_from(V,N))
#define check_path(A,B) BOOST_CHECK(has_path(A,B))

BOOST_FIXTURE_TEST_CASE(bi_dir_works, NavFixture) {
	map.add_relative({DOWN,DOWN});
	check_from(DOWN,1);
	check_from(DOWN+DOWN,1);
}

BOOST_AUTO_TEST_CASE(vector_comparisons) {
	MeterVector a{0.f,-1.f},
	 	 b{1.f,-1.f},
	 	 c{1.f,-1.f},
	 	 d{0.f, 0.f};
	BOOST_REQUIRE(a < b);
	BOOST_REQUIRE(!(b < a));
	BOOST_REQUIRE(!(c < d));
	BOOST_REQUIRE(d < c);
}

BOOST_FIXTURE_TEST_CASE(add_and_query, NavFixture) {
	add_all_dirs(DOWN);
	check_from(DOWN,4);
	check_from(DOWN+DOWN,1);
}

BOOST_FIXTURE_TEST_CASE(add_and_query_duplicates, NavFixture) {
	add_all_dirs(DOWN);
	add_all_dirs(DOWN);
	check_from(DOWN,4);
	check_from(DOWN+DOWN,1);
	check_path(DOWN+DOWN,DOWN);
}

BOOST_FIXTURE_TEST_CASE(add_and_query_2, NavFixture) {
	add_all_dirs(ORIGIN);
	add_all_dirs(DOWN);
	check_from(DOWN,4);
	check_from(DOWN+DOWN,1);
}

BOOST_FIXTURE_TEST_CASE(astar_search, NavFixture) {
	add_all_dirs(ORIGIN);
	auto end_point = DOWN+DOWN;
	map.add_absolute({LEFT,end_point});
	map.add_absolute({DOWN,end_point});
	map.add_absolute({RIGHT,end_point});
	auto path = map.astar(ORIGIN,end_point);
	decltype(path) expected{ORIGIN,DOWN,end_point};
	BOOST_CHECK_EQUAL(path,expected);

}

BOOST_AUTO_TEST_SUITE_END()
}
