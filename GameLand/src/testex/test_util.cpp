// see licence.txt

#include "test_util.h"
#include <cstring>
#include "../systemex/systemex.h"
namespace tut {
	using namespace systemex;
	void ensure_contains(const char * text, const char * sought) {
		char * fnd = std::strstr(text,sought);
		if (fnd == 0) {
			auto msg = string_from_format("could not find '%s' in '%s'",sought,text);
			fail(msg.c_str());
		}
	}

	void ensure_contains(const std::exception & error, const char * sought) {
		ensure_contains(error.what(), sought);
	}

	void ensure_equalsf(const char * text, const float& found, const float &expected) {
		if (fabs(found - expected) > 0.0001)
			ensure_equals(text,expected,found);
	}

}
