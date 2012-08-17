#include <tut/tut.hpp>
#include "systemex.h"
/**
 * \brief Unit tests
 */
namespace tut {
	using namespace systemex;
	struct data {
	};
	test_group<data> systemTests("010 System Tests");

	template<> template<> void test_group<data>::object::test<1>() {
		runtime_error_ex ex("hello %s", "dolly");
		ensure_equals("message format with one string", string(ex.what()),
				"hello dolly");
	}

	template<> template<> void test_group<data>::object::test<2>() {
		runtime_error_ex ex("hello %s how is %s", "dolly", "molly");
		ensure_equals("message format with two strings", string(ex.what()),
				"hello dolly how is molly");
	}

}
;
