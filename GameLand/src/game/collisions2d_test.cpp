/**
 * Copyright 2013 Willem Duminy
 * See LICENCE.txt
 */
#include "../testex/test_util.h"
#include "../systemex/systemex.h"
#include "collisions2d.h"
#include <sstream>
#define TEST(Num,Name,Logic) template<> template<> void test_group<CollisionData>::object::test<Num>() {set_test_name(Name); Logic}
#define BEGIN(Num,Name) template<> template<> void test_group<CollisionData>::object::test<Num>() {set_test_name(Name);
#define END }



namespace tut {
	using namespace game;
	using game::Polygon;

	class Diamond : public CollidablePolygon {
	public:
		Diamond(const Vector2 top) : CollidablePolygon(0,top) {
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
		CollisionData() : mgr(CollisionListener::u_ptr(new TestListener())) {};
		virtual ~CollisionData(){}
		const TestListener& listener() const { return (TestListener&)  mgr.listener();}
		CollisionManager mgr;

	};

	test_group<CollisionData> collisionTests("020 Collision Subsystem");

	void ensure_overlap_bounds(const BoundedBox2& a, const BoundedBox2& b) {
		_ensure("Expected overlap on " << a << " and " << b, a.in_bounds_of(b));
	}

	void ensure_not_overlap_bounds(const BoundedBox2& a, const BoundedBox2& b) {
		_ensure_not("Not expected box overlap on " << a << " and " << b, a.in_bounds_of(b));
	}

	void ensure_not_overlap(const Polygon& a, const Polygon& b) {
		_ensure_not("Polygons not expected overlap on " << a << " and " << b,
				a.overlaps_with(b));
	}

	void ensure_overlap(const Polygon& a, const Polygon& b) {
		_ensure("Polygons expected overlap on " << a << " and " << b,
				a.overlaps_with(b));
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
		ensure("d1 not added", mgr.add_if_not_collide(&d1));
		ensure("d2 added", !mgr.add_if_not_collide(&d2));
		ensure(listener().collide_count == 1);
	} END


	BEGIN(9, "polygons with one point must not overlap") {
		CollidablePolygon a(0,Vector::origin);
		CollidablePolygon b(0,Vector(0.07,1,0));
		b.add(Vector(-0.07,1,0));
		b.add(Vector(0,0,0));
		ensure_not_overlap_bounds(a,b);
		ensure_not_overlap(a,b);
	} END


}
