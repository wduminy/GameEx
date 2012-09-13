/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#pragma once
#include <tut/tut.hpp>
/**
 * Template unit test framework, see <http://tut-framework.sourceforge.net/>
 */

#define ensure_error(C,E) try {C;fail("This test expected an exception");} catch(tut::failure &f){throw;} catch (std::exception &ex) {ensure_contains(ex, E);}

namespace tut {
	void ensure_contains(const char * text, const char * sought);
	void ensure_contains(const std::exception & error, const char * sought);
	void ensure_equalsf(const char * text, const float& found, const float &expected);
}
