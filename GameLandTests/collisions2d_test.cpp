/**
 * Copyright 2013 Willem Duminy
 * See LICENCE.txt
 */
#include <test_util.h>
#include <systemex.h>
#include <collisions2d.h>
#include <sstream>
#define TEST(Num,Name,Logic) template<> template<> void test_group<CollisionData>::object::test<Num>() {set_test_name(Name); Logic}
#define BEGIN(Num,Name) template<> template<> void test_group<CollisionData>::object::test<Num>() {set_test_name(Name);
#define END }

enum class game::object_t {Dummy};

namespace tut {
	using namespace game;
	using game::Polygon;
	using game::object_t;
	class Diamond : public CollidablePolygon {
	public:
		Diamond(const Vector2 top) : CollidablePolygon(object_t::Dummy,top) {
			add_relative(Vector2(1,-1));
			add_relative(Vector2(-1,-1));
			add_relative(Vector2(-1,1));
		}
	};

	class TestListener : public CollisionListener {
	public:
		TestListener() : collide_count(0) {};
		void on_collide(game::CollidablePolygon& a, game::CollidablePolygon& b) override {
			collide_count++;
		}
		int collide_count;
	};

	Vector2 diag_one = Vector2(unity,unity);
	BoundedBox2 box(const Vector2& leftTop) {
		return BoundedBox2(leftTop, leftTop + diag_one);
	}

	struct CollisionData {
		CollisionData() : mgr(new TestListener()) {};
		virtual ~CollisionData(){}
		const TestListener& listener() const { return (TestListener&)  mgr.listener();}
		SimpleCollisionManager mgr;

	};

	test_group<CollisionData> collisionTests("020 Collision Subsystem");

	void ensure_overlap_bounds(const BoundedBox2& a, const BoundedBox2& b) {
		_ensure("Expected overlap on " << a << " and " << b, a.in_bounds_of(b));
	}

	void ensure_not_overlap_bounds(const BoundedBox2& a, const BoundedBox2& b) {
		_ensure_not("Not expected box overlap on " << a << " and " << b, a.in_bounds_of(b));
	}

	void ensure_overlap_bounds(const CollidablePolygon& a, const CollidablePolygon& b) {
		ensure_overlap_bounds(a.box(),b.box());
	}

	void ensure_not_overlap_bounds(const CollidablePolygon& a, const CollidablePolygon& b) {
		ensure_not_overlap_bounds(a.box(),b.box());
	}

	void ensure_not_overlap(const Polygon& a, const Polygon& b) {
		_ensure_not("Polygons not expected overlap on " << a << " and " << b,
				a.overlaps_with(b));
	}

	void ensure_overlap(const Polygon& a, const Polygon& b) {
		_ensure("Polygons expected overlap on " << a << " and " << b,
				a.overlaps_with(b));
	}

	void ensure_not_overlap(const CollidablePolygon& a, const CollidablePolygon& b) {
		ensure_not_overlap(a.poly(),b.poly());
	}

	void ensure_overlap(const CollidablePolygon& a, const CollidablePolygon& b) {
		ensure_overlap(a.poly(),b.poly());
	}

	BEGIN(1, "boxes with overlapping boundaries must overlap")
		BoundedBox2 a = box(Vector2::origin);
		BoundedBox2 b = box(diag_one / 2.0f);
		BoundedBox2 c = box(diag_one * 2.0f);
		ensure_overlap_bounds(a,a);
		ensure_overlap_bounds(a,b);
	END

	BEGIN(2, "boxes that are removed from each other must not overlap")
		BoundedBox2 a = box(Vector2::origin);
		BoundedBox2 c = box(diag_one * 2.0f);
		ensure_not_overlap_bounds(a,c);
	END

	BEGIN(3, "diamonds that overlaps must report overlap") {
		Diamond d1(Vector2(0,0)), d2(Vector2(0.5,0.5));
		ensure_overlap(d1,d2);
	} END

	BEGIN(4, "diamonds close to each other must not overlap") {
		Diamond d1(Vector2(0,0)), d2(Vector2(1.0,1.01));
		ensure_overlap_bounds(d1,d2);
		ensure_not_overlap(d1,d2);
	} END

	BEGIN(5, "diamonds interlacing on the right must overlap") {
		Diamond d1(Vector2(0,0)), d2(Vector2(0.9,0));
		ensure_overlap(d1,d2);
	} END

	BEGIN(6, "diamonds interlacing on the left must overlap") {
		Diamond d1(Vector2(0,0)), d2(Vector2(-0.9,0));
		ensure_overlap(d1,d2);
	} END

	BEGIN(7, "diamonds interlacing on below must overlap") {
		Diamond d1(Vector2(0,0)), d2(Vector2(0,0.9));
		ensure_overlap(d1,d2);
	} END

	BEGIN(8, "overlapping diamonds must collide") {
		Diamond d1(Vector2(0,0)), d2(Vector2(0,0.9));
		ensure("d1 not collided", mgr.check_and_add(&d1));
		ensure("d2 collided", !mgr.check_and_add(&d2));
		ensure(listener().collide_count == 1);
	} END


	BEGIN(9, "polygons with one point must not overlap") {
		CollidablePolygon a(object_t::Dummy,Vector::origin);
		CollidablePolygon b(object_t::Dummy,Vector2(0.07,1),{{-0.07,1},{0,0}});
		ensure_not_overlap_bounds(a,b);
		ensure_not_overlap(a,b);
	} END

	BEGIN(10, "this specific case must overlap") {
		// 7:> adding {4:(0.13547,0.00201677,0)(0.234465,-0.0969783,0)(0.319318,-0.0121256,0)(0.220323,0.0868695,0)} <:
		CollidablePolygon a(object_t::Dummy,Vector(0.13547,0.00201677,0),
		{{0.234465,-0.0969783},
		{0.319318,-0.0121256},
		{0.220323, 0.0868695}});
		// 7:> collides with {4:(0.206181,0.0727274,0)(0.305176,-0.0262677,0)(0.390029,0.058585,0)(0.291034,0.15758,0)} <:
		CollidablePolygon b(object_t::Dummy,Vector(0.206181,0.0727274,0),
		{{0.305176,-0.0262677},
		{0.390029, 0.058585},
		{0.291034, 0.15758}});
		ensure_overlap_bounds(a,b);
		ensure_overlap(a,b);
	} END

	BEGIN(11, "this specific case must overlap") {
		CollidablePolygon a(object_t::Dummy,Vector(-0.070,-0.200, 0.000)
			,{{ 0.070,-0.200},{ 0.070,-0.080},{-0.070,-0.080}});
		CollidablePolygon b(object_t::Dummy,Vector(-0.070,-0.100, 0.000)
			,{{ 0.070,-0.100},{ 0.070, 0.020},{-0.070, 0.020}});
		ensure_overlap_bounds(a,b);
		ensure_overlap(a,b);
	} END
}
