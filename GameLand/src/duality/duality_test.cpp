#include "../testex/test_util.h"
#include "snake.h"

namespace tut {
	using namespace duality;
	using namespace game;
	struct Snakedata {
	    virtual ~Snakedata(){}
    };
	test_group<Snakedata> snakeTests("100 Snake tests");
	#define SNAKE_TEST(number) template<> template<> void test_group<Snakedata>::object::test<number>()

	SNAKE_TEST(1) {
		Snake s(Vector::origin, Vector::north);
		s.move();
		ensure_equals(s.size(),1);
	}

	SNAKE_TEST(2) {
		Snake s(Vector::origin, Vector::north);
		ensure_equals(s.head(),s.tail());
		s.move();
		ensure("head and tail must be different after move",s.head() != s.tail());
	}

	SNAKE_TEST(3) {
		Snake s(Vector::origin, Vector::north,unity);
		s.move();
		s.move();
		ensure_equals("move is not two steps forward",(Vector::north * 2.0f), s.head());
	}

	SNAKE_TEST(4) {
		Snake s(Vector::origin, Vector::north,unity,degrees45 * 2.0f);
		s.move();
		ensure_equals("forward end up at wrong place", s.head(),Vector(0,1,0) );
		s.move(Left);
		ensure_equals("f-l end up at wrong place", s.head(),Vector(-1,1,0) );
		s.move();
		ensure_equals("f-l-f end up at wrong place", s.head(),Vector(-2,1,0) );
		s.move(Left);
		ensure_equals("second left ends up at wrong place", s.head(),Vector(-2,0,0) );
	}

	SNAKE_TEST(5) {
		Snake s(Vector::origin, Vector::north,unity,degrees45 * 2.0f);
		s.move();
		s.move(Right);
		ensure_equals("f-r end up at wrong place", s.head(),Vector(1,1,0) );
		s.move();
		s.move(Right);
		ensure_equals("f-r-f-r ends up at wrong place", s.head(),Vector(2,0,0) );
	}

}
