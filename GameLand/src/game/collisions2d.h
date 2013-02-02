/**
 * Copyright 2013 Willem Duminy
 * See LICENCE.txt
 */
#pragma once
#include "game_math.h"
#include <vector>
namespace game {
	using std::vector;
/**
 * A bounded box has a left-top and a right-bottom location
 */
class BoundedBox2 {
public:
	BoundedBox2(const Vector2& lt, const Vector2& rb);
	BoundedBox2() : BoundedBox2(Vector2::origin, Vector2::origin) {}
	const Vector2 left_top() const {return _lt;}
	const Vector2 right_bottom() const {return _rb;}
	virtual ~BoundedBox2() {}
	bool in_bounds_of(const BoundedBox2& other) const;
	bool is_valid() const;
protected:
	void adjust_box(const Vector2& p);
private:
	const Scalar left() const {return _lt.x();}
	const Scalar right() const {return _rb.x();}
	const Scalar bottom() const {return _rb.y();}
	const Scalar top() const {return _lt.y();}
	Vector2 _lt;
	Vector2 _rb;
public:
	friend ostream& operator<<(ostream& s, const BoundedBox2& v);
};

/**
 * A two dimensional figure with at least one point
 */
class Polygon {
public:
	Polygon(const Vector2& start) : _points() {add(start);}
	virtual void add(const Vector2& value);
	void add_relative(const Vector2& value);
	/**
	 * This method assumes both polygons are convex.
	 * @param other
	 * @return
	 */
	bool overlaps_with(const Polygon &other) const;
	/**
	 * The axis are the normals for each edge in the polygon.
	 * Note the the resulting vectors are not normalised
	 * @return
	 */
	vector<Vector2> axes() const;
	Range project(const Vector2 &axis) const;
	virtual ~Polygon() {}
private:
	vector<Vector2> _points;
public:
	friend ostream& operator<<(ostream& s, const Polygon& v);
};

/**
 * If a collidable is updated, the collision manager must be told about it
 */
class CollidablePolygon : public BoundedBox2, public Polygon {
public:
	CollidablePolygon(const Vector2& start);
	void add(const Vector2& value) override;
	bool collides_with(const CollidablePolygon& other) const;
	virtual ~CollidablePolygon() {}
};

class CollisionManager {

};

}
