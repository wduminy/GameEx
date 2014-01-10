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
 * A two dimensional bound. It has a left-top and a right-bottom location
 */
class BoundedBox2 {
public:
	/**
	 * Constructor for left top and right bottom.
	 */
	BoundedBox2(const Vector2& lt, const Vector2& rb);
	/**
	 * Constructor for left top and size dimensions.
	 */
	BoundedBox2(const Scalar x, const Scalar y, const Scalar w, const Scalar h) : BoundedBox2(Vector2(x,y),Vector2(x+w,y+h)) {}
	BoundedBox2() : BoundedBox2(Vector2::origin, Vector2::origin) {}
	const Vector2 left_top() const {return _lt;}
	const Vector2 right_bottom() const {return _rb;}
	virtual ~BoundedBox2() {}
	bool in_bounds_of(const BoundedBox2& other) const;
	bool is_valid() const;
    bool is_empty_box() const {return left() == right() || top() == bottom();}
	const Scalar left() const {return _lt.x();}
	const Scalar right() const {return _rb.x();}
	const Scalar bottom() const {return _rb.y();}
	const Scalar top() const {return _lt.y();}
protected:
	/** Makes the box bigger so that the point p fits into it */
	void adjust_box(const Vector2& p);
	/** Makes the box a point particle.
	 * @param v is the location of the point */
	void clear_box(const Vector2& v) {_lt = _rb = v;}
private:
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
	void set_start(const Vector2& start); 
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
	virtual void on_clear(const Vector2& initial) {}
public:
	friend ostream& operator<<(ostream& s, const Polygon& v);
};

/**
 * A Polygon with a BoundedBox2.  
 * It is typically added to CollisionManager.  
 */
class CollidablePolygon : public BoundedBox2, public Polygon {
public:
	CollidablePolygon(const unsigned char type, const Vector2& start);
	CollidablePolygon(const unsigned char type, const Vector2& start,  const vector<Vector2> &path);
	bool collides_with(const CollidablePolygon& other) const;
	unsigned char type() const {return _type;}
	virtual ~CollidablePolygon() {}
protected:
	void on_add(const Vector2& value) override;
	void on_clear(const Vector2& initial) override;
private:
	const unsigned char _type;
public:
	typedef std::unique_ptr<CollidablePolygon> u_ptr;
};

inline ostream& operator<<(ostream& s, const CollidablePolygon& v) {
	return s << dynamic_cast<const Polygon&> (v);
}

/** Listens to collisions */
class CollisionListener {
public:
	virtual void on_collide(CollidablePolygon &a, CollidablePolygon &b) {};
	virtual ~CollisionListener() {}
public:
	typedef std::unique_ptr<CollisionListener> u_ptr;
};

class CollidablePolygonPList : public std::list<CollidablePolygon *> {
public:
	CollidablePolygon * collider_or_null(CollidablePolygon * collidable);
	virtual ~CollidablePolygonPList() {}
};

class CollisionManager {
public:
	CollisionManager(CollisionListener * listener);
	/** Checks for a collision and adds object.  \
	  * This method is the one that causes collision events 
	  */
	bool check_and_add(CollidablePolygon * collidable, const bool ignoreIfCollide = false);
	virtual void add(CollidablePolygon * collidable) = 0;
	virtual void remove(CollidablePolygon * collidable) = 0;
	virtual void clear() = 0;
	const CollisionListener& listener() const {return *_listener;}
	virtual ~CollisionManager(){}
protected:
	virtual CollidablePolygon * collider_or_null(CollidablePolygon * collidable) = 0;
private:
	CollisionListener::u_ptr _listener;
public:
	typedef std::unique_ptr<CollisionManager> u_ptr;
};



class SimpleCollisionManager : public CollisionManager {
public:
	SimpleCollisionManager(CollisionListener * listener);
	void add(CollidablePolygon * collidable) override;
	void remove(CollidablePolygon * collidable) override;
	void clear() override {_items.clear();}
protected:
	CollidablePolygon * collider_or_null(CollidablePolygon * collidable) override;
private:
	std::tuple<int,int> box_of(CollidablePolygon * collidable);
	CollidablePolygonPList _items;

};

class BoxRange {
public:
	BoxRange(int xs, int xe, int ys, int ye) :
		_xs(xs), _xe(xe), _ys(ys), _ye(ye) {}
	const int _xs;
	const int _xe;
	const int _ys;
	const int _ye;
};

/** A collision managers that resolves collisions using boxes.
 * Essentially the ground is divided into equally sized boxes.  When checking for a collision
 * related boxes are checked - not all objects in the manager.  Keep in mind that an object may be in 
 * many boxes -- because it is larger than a box, or on a box boundary.
 * \sa \ref using_collision_mgr
 */
class CollisionManagerWithBoxes : public CollisionManager {
public:
	/**
	 * Construct a CollisionManagerWithBoxes.
	 * \param listener is called when a collision occurs (see check_and_add()).  This instance is owned by the manager. Cannot be null.
	 * \param bounds is a BoundedBox2 that determines the boxed area.  Object may still be placed out of bounds; but collision checking will note be optimal for these.
	 * \param division_count defines how many boxes will be created.  Tweak this for your game -- how many objects is there in a box? 
	 */
	CollisionManagerWithBoxes(CollisionListener * listener, const BoundedBox2 & bounds, const unsigned int division_count);
	void add(CollidablePolygon * collidable) override;
	void remove(CollidablePolygon * collidable) override;
	void clear() override;
protected:
	CollidablePolygon * collider_or_null(CollidablePolygon * collidable) override;
	CollidablePolygonPList& list_at(int x, int y) {return _boxes[y * _division_count + x]; }
	void modify_boxes(CollidablePolygon * collidable, const bool do_add);
private:
	int box_x(const Scalar x) { return (x - _bounds.left()) / _division_count; }
	int box_y(const Scalar y) { return (y - _bounds.top()) / _division_count; }
	BoxRange range_from(CollidablePolygon * p);
	CollidablePolygonPList _outside_items;
	const BoundedBox2 _bounds;
	vector<CollidablePolygonPList> _boxes;
	const int _division_count;
};

}
