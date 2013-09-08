// #include "../testex/test_util.h"
// #include "snake.h"

// namespace tut {
// 	using namespace duality;
// 	using namespace game;
// 	struct Snakedata {
// 		Snakedata() : mgr(CollisionListener::u_ptr(new CollisionListener())) {}
// 	    virtual ~Snakedata(){}
// 	    SimpleCollisionManager mgr;
//     };
// 	test_group<Snakedata> snakeTests("100 Duality tests");
// #define SNAKE_TEST(N) template<> template<> void test_group<Snakedata>::object::test<N>()
// #define BEGIN(Num,Name) template<> template<> void test_group<Snakedata>::object::test<Num>() {set_test_name(Name);
// #define END }

// 	SpinePoint::u_ptr spine(const Vector& v) {
// 		SpinePoint::u_ptr r(new SpinePoint());
// 		r->assign(v);
// 		return r;
// 	};

// 	SNAKE_TEST(1) {
// 		Snake s(Vector::origin, Vector::north, 1);
// 		s.move();
// 		ensure_equals(s.size(),2U);
// 	}

// 	SNAKE_TEST(2) {
// 		Snake s(Vector::origin, Vector::north, 1);
// 		ensure("head and tail must be different after create",s.head().topMiddle() != s.tail().topMiddle());
// 	}

// 	SNAKE_TEST(3) {
// 		Snake s(Vector::origin, Vector::north,unity,1);
// 		s.move();
// 		ensure_equals("move is not two steps forward",(Vector::north * 2.0f), s.head().topMiddle());
// 	}

// 	SNAKE_TEST(4) {
// 		Snake s(Vector::origin, Vector::north,1,unity,degrees45 * 2.0f);
// 		s.move(Left);
// 		ensure_equals("f-l end up at wrong place", s.head().topMiddle(), Vector::north + Vector::west );
// 		s.move();
// 		ensure_equals("f-l-f end up at wrong place", s.head().topMiddle(), Vector::north + Vector::west * 2.0f );
// 		s.move(Left);
// 		ensure_equals("second left ends up at wrong place", s.head().topMiddle(), Vector::west * 2.0f );
// 	}

// 	SNAKE_TEST(5) {
// 		Snake s(Vector::origin, Vector::north,1,unity,degrees45 * 2.0f);
// 		s.move(Right);
// 		ensure_equals("f-r end up at wrong place", s.head().topMiddle(), Vector::north + Vector::east );
// 		s.move();
// 		s.move(Right);
// 		ensure_equals("f-r-f-r ends up at wrong place", s.head().topMiddle(), Vector::east * 2.0f );
// 	}

// 	SNAKE_TEST(6) {
// 		Snake s;
// 		const unsigned int increment = 5;
// 		while (s.is_growing())
// 			s.move();
// 		auto oldSize = s.size();
// 		s.grow(increment);
// 		while (s.is_growing())
// 				s.move();
// 		ensure_equals("growth not correct", s.size(), oldSize + increment);
// 		s.move();
// 		s.move();
// 		ensure_equals("growing too much", s.size(), oldSize + increment);
// 	}

// 	SNAKE_TEST(7) {
// 		Snake s;
// 		const unsigned int increment = SNAKE_MEM_SIZE;
// 		s.grow(SNAKE_MEM_SIZE);
// 		while (s.is_growing())
// 			s.move();
// 		ensure_equals("not max size",s.size(), SNAKE_MEM_SIZE-1);
// 		s.grow(increment);
// 		ensure("must not grow beyond memory bounds", !s.is_growing() );
// 	}

// 	SNAKE_TEST(8) {
// 		SpinePoint p;
// 		auto b = spine(Vector::north);
// 		p.assign(Vector::origin,b.get());
// 		ensure_equals("left is not w",Vector::west*SNAKE_WIDTH_HALF,p.bottomLeft());
// 		ensure_equals("right is not e",Vector::east*SNAKE_WIDTH_HALF,p.bottomRight());
// 	}

// 	SNAKE_TEST(9) {
// 		SpinePoint p;
// 		Vector start = Vector::north;
// 		auto b = spine(Vector::origin);
// 		p.assign(Vector::north,b.get());
// 		ensure_equals("left is not e",Vector::east*SNAKE_WIDTH_HALF + start,p.bottomLeft());
// 		ensure_equals("right is not w",Vector::west*SNAKE_WIDTH_HALF + start,p.bottomRight());
// 	}

// 	void ensure_no_overlap(SpinePoint::u_ptr& a, SpinePoint::u_ptr& b) {
// 		_ensure_not("unexpected polygon overlap: " << a->polygon() << " and " << b->polygon(),  a->polygon().overlaps_with(b->polygon()));
// 	}

// 	void ensure_overlap(SpinePoint::u_ptr& a, SpinePoint::u_ptr& b) {
// 		_ensure("Expected overlap: " << a->polygon() << " and " << b->polygon(),  a->polygon().overlaps_with(b->polygon()));
// 	}


// 	BEGIN(10, "Consecutive spine polygons must not overlap") {
// 		auto a = spine(Vector::origin);
// 		auto b = spine(Vector::north);
// 		b->assign(Vector::north, a.get());
// 		ensure_no_overlap(a,b);
// 		ensure_no_overlap(b,a);
// 	} END

// 	BEGIN(11, "Second spine must produce correct polygon") {
// 		auto a = spine(Vector::origin);
// 		auto b = spine(Vector::origin);
// 		b->assign(Vector::north, a.get());
// 		auto p = b->polygon();
// 		ensure_not(p.is_empty());
// 		ensure_not(p[0] == p[1]);
// 	} END


// 	BEGIN(12, "Third spine must produce correct polygon") {
// 		auto a = spine(Vector::origin);
// 		auto b = spine(Vector::origin);
// 		auto c = spine(Vector::origin);
// 		b->assign(Vector::north, a.get());
// 		c->assign(Vector::north*2, b.get());
// 		ensure_not(c->polygon().is_empty());
// 	} END


// 	BEGIN(13, "Snake object should move north without colliding") {
// 		SnakeWithCollision o(mgr);
// 		o.move(SteerDirection::Forward);
// 		ensure(o.is_alive());
// 	} END

// 	BEGIN(14, "Snake object that runs in a circle must collide") {
// 		SnakeWithCollision o(mgr);
// 		for (int i=0;i<100;i++)
// 			o.move(SteerDirection::Right);
// 		ensure(!o.is_alive());
// 	} END

// 	BEGIN(15, "Snake object should move south without colliding") {
// 		SnakeWithCollision o(mgr,Vector::north + (Vector::up * (SNAKE_WIDTH_HALF)),
// 				             Vector::south);
// 		for (int i=0;i<100;i++)
// 			o.move(SteerDirection::Forward);
// 		ensure(o.is_alive());
// 	} END
// }
