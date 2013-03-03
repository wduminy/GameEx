/**
 * Copyright 2013 Willem Duminy
 * See LICENCE.txt
 */
#pragma once
#include "game_math.h"
#include <vector>
#include <list>
namespace game {
	using std::list;
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
    bool is_empty_box() const {return left() == right() || top() == bottom();}
protected:
	void adjust_box(const Vector2& p);
	void clear_box() {_lt = Vector2::origin; _rb = Vector2::origin;}
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
	void add(const Vector2& value);
	void set_start(const Vector2& start) {_points.clear();on_clear();add(start);}
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
	list<Vector2> axes() const;
	Range project(const Vector2 &axis) const;
	virtual ~Polygon() {}
	size_t point_count() const {return _points.size();}
	bool is_empty() const {return point_count() < 3; }
	const Vector2&  operator[](size_t i) const  {return _points[i];}
private:
	vector<Vector2> _points;
protected:
	virtual void on_add(const Vector2& value) {}
	virtual void on_clear() {}
public:
	friend ostream& operator<<(ostream& s, const Polygon& v);
};

/**
 * If a collidable is updated, the collision manager must be told about it
 */
class CollidablePolygon : public BoundedBox2, public Polygon {
public:
	CollidablePolygon(const unsigned char type, const Vector2& start);
	bool collides_with(const CollidablePolygon& other) const;
	unsigned char type() const {return _type;}
	virtual ~CollidablePolygon() {}
protected:
	void on_add(const Vector2& value) override;
	void on_clear() override;
private:
	const unsigned char _type;
public:
	typedef std::unique_ptr<CollidablePolygon> u_ptr;
};

inline ostream& operator<<(ostream& s, const CollidablePolygon& v) {
	return s << dynamic_cast<const Polygon&> (v);
}

class CollisionListener {
public:
	virtual void on_collide(CollidablePolygon &a, CollidablePolygon &b) {};
	virtual ~CollisionListener() {}
public:
	typedef std::unique_ptr<CollisionListener> u_ptr;
};

class CollisionManager {
public:
	CollisionManager(CollisionListener::u_ptr listener);
	/**
	 * Adds a new polygon if no collision is detected
	 * @param collidable
	 * @return true if the polygon is added
	 */
	bool add_if_not_collide(CollidablePolygon * collidable);
	void remove(CollidablePolygon * collidable);
	const CollisionListener& listener() const {return *_listener;}
private:
	CollisionListener::u_ptr _listener;
public:
	typedef std::unique_ptr<CollisionManager> u_ptr;
	std::list<CollidablePolygon *> _items;
};

}
