/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#pragma once
#include <tut/tut.hpp>
#include <sstream>
/**
 * Template unit test framework, see <http://tut-framework.sourceforge.net/>
 */

#define ensure_error(C,E) try {C;fail("This test expected an exception");} catch(tut::failure &f){throw;} catch (std::exception &ex) {ensure_contains(ex, E);}
#define _fail(M) {std::ostringstream msg;msg << M;fail_with(msg);}
#define _ensure(M,B) {if (!B) _fail(M)};
#define _ensure_not(M,B) {if (B) _fail(M)};
namespace tut {
	void ensure_contains(const char * text, const char * sought);
	void ensure_contains(const std::exception & error, const char * sought);
	void ensure_equalsf(const char * text, const float& found, const float &expected);
	void fail_with(const std::ostringstream& s);
}
