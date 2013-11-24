/**
 * Copyright 2013 Willem Duminy
 * See LICENCE.txt
 */
#include "collisions2d.h"
#include "log.h"
namespace game {
bool BoundedBox2::in_bounds_of(const BoundedBox2& other) const {
	if (is_empty_box() || other.is_empty_box())
		return false;
	else
		return overlap_or_equal(left(), right(), other.left(), other.right())
				&& overlap_or_equal(top(), bottom(), other.top(),
						other.bottom());
}

void Polygon::add(const Vector2& value) {
	ASSERT(_points.size() == 0 || value != _points.back());
	_points.push_back(value);
	on_add(value);
}

void Polygon::add_relative(const Vector2& value) {
	add(_points.back() + value);
}

bool axis_overlap(const Polygon &p1, const Polygon &p2) {
	if (p1.is_empty() || p2.is_empty())
		return false;
	auto axlist = p1.axes();
	for (auto axis = axlist.begin(); axis != axlist.end(); axis++) {
		const Range pp1 = p1.project(*axis);
		const Range pp2 = p2.project(*axis);
		if (!pp1.overlaps_or_equal(pp2))
			return false;
	}
	return true;
}

/**
 * Here I use the separating axis theorem.
 * For more info see http://www.codezealot.org/archives/55
 * @param other
 * @return
 */
bool Polygon::overlaps_with(const Polygon& other) const {
	bool r = axis_overlap(*this, other);
	if (r)
		r = axis_overlap(other, *this);
	return r;
}

CollidablePolygon::CollidablePolygon(const unsigned char type,
		const Vector2& start) :
		BoundedBox2(start, start), Polygon(start), _type(type) {
}

CollidablePolygon::CollidablePolygon(const unsigned char type,
		const Vector2& start, const vector<Vector2> &path) :
		BoundedBox2(start, start), Polygon(start), _type(type) {
	for_each(e,path)
	{
		add(*e);
	}
}

bool CollidablePolygon::collides_with(const CollidablePolygon& other) const {
	if (in_bounds_of(other))
		return overlaps_with(other);
	else
		return false;
}

BoundedBox2::BoundedBox2(const Vector2& lt, const Vector2& rb) :
		_lt(lt), _rb(rb) {
}

bool BoundedBox2::is_valid() const {
	return (left() <= right()) && (top() <= bottom());
}

void BoundedBox2::adjust_box(const Vector2& p) {
	if (p.x() < _lt.x())
		_lt.set_x(p.x());
	if (p.x() > _rb.x())
		_rb.set_x(p.x());
	if (p.y() < _lt.y())
		_lt.set_y(p.y());
	if (p.y() > _rb.y())
		_rb.set_y(p.y());
}

std::ostream& operator <<(ostream& s, const BoundedBox2& v) {
	s << "{" << v._lt << v._rb << "}";
	return s;
}

list<Vector2> Polygon::axes() const {
	if (_points.size() < 2)
		return list<Vector2>();
	const auto size = _points.size() - 1;
	list<Vector2> result;
	auto previous_index = size;
	for (auto i = 0U; i < size; i++) {
		const Vector2& a = _points[previous_index];
		const Vector2& b = _points[i];
		auto r = (b - a).perpendicular();
		result.push_back(r);
		previous_index = i;
	}
	return result;
}

Range Polygon::project(const Vector2& axis) const {
	if (_points.size() == 0)
		return Range(zero, zero);
	auto min = axis.dot2(_points[0]);
	auto max = min;
	for (auto i = 1U; i < _points.size(); i++) {
		auto r = axis.dot2(_points.at(i));
		if (r < min)
			min = r;
		if (r > max)
			max = r;
	}
	return Range(min, max);
}

void CollidablePolygon::on_add(const Vector2& value) {
	adjust_box(value);
}

void CollidablePolygon::on_clear() {
	clear_box();
}
std::ostream& operator <<(ostream& s, const Polygon& v) {
	s << "{" << v._points.size() << ":";
	for_each(p, v._points)
	{
		s << *p;
	}
	s << "}";
	return s;
}

CollisionManager::CollisionManager(CollisionListener* listener) :
		_listener(listener) {
}

bool CollisionManager::check_and_add(CollidablePolygon* collidable,
		const bool ignoreIfCollide) {
	auto other = collider_or_null(collidable);
	if (other != 0) {
		_listener->on_collide(*other, *collidable);
		if (!ignoreIfCollide)
			add(collidable);
		return false;
	} else {
		add(collidable);
		return true;
	}
}

SimpleCollisionManager::SimpleCollisionManager(
		CollisionListener * listener) :
		CollisionManager(listener), _items() {
}

void SimpleCollisionManager::remove(CollidablePolygon* collidable) {
	_items.remove(collidable);
}

void SimpleCollisionManager::add(CollidablePolygon* collidable) {
	_items.push_back(collidable);
}

CollidablePolygon * SimpleCollisionManager::collider_or_null(
		CollidablePolygon * collidable) {
	return _items.collider_or_null(collidable);
}

CollidablePolygon* CollidablePolygonPList::collider_or_null(
		CollidablePolygon* collidable) {
	ASSERT(collidable->point_count() > 0);
	ASSERT(collidable != 0);
	for_each(e, (*this))
	{
		if ((*e)->collides_with(*collidable))
			return *e;
	}
	return 0;
}

CollisionManagerWithBoxes::CollisionManagerWithBoxes(
		CollisionListener * listener, const BoundedBox2 & bounds,
		const unsigned int division_count) :
		CollisionManager(std::move(listener)), _bounds(bounds), _boxes(
				division_count * division_count), _division_count(
				division_count) {

}

void CollisionManagerWithBoxes::remove(CollidablePolygon* collidable) {
	modify_boxes(collidable, false);
}

void CollisionManagerWithBoxes::clear() {
	_outside_items.clear();
	for_each(b, _boxes) {(*b).clear();}
}

void CollisionManagerWithBoxes::add(CollidablePolygon* collidable) {
	modify_boxes(collidable, true);
}

void CollisionManagerWithBoxes::modify_boxes(CollidablePolygon* collidable,
		const bool do_add) {
	const auto range = range_from(collidable);
	bool out_of_bounds = false;
	for (int x = range._xs; x <= range._xe; x++) {
		if (x < 0 || x >= _division_count)
			out_of_bounds = true;
		else
			for (int y = range._ys; y <= range._ye; y++)
				if (y < 0 || y >= _division_count)
					out_of_bounds = true;
				else {
					if (do_add)
						list_at(x, y).push_back(collidable);
					else
						list_at(x, y).remove(collidable);
				}
	}
	if (out_of_bounds) {
		if (do_add)
			_outside_items.push_back(collidable);
		else
			_outside_items.remove(collidable);
	}
}

BoxRange CollisionManagerWithBoxes::range_from(CollidablePolygon* collidable) {
	return BoxRange(box_x(collidable->left()), box_x(collidable->right()),
			box_y(collidable->top()), box_y(collidable->bottom()));
}

CollidablePolygon * CollisionManagerWithBoxes::collider_or_null(
		CollidablePolygon * collidable) {
	const auto range = range_from(collidable);
	bool out_of_bounds = false;
	for (int x = range._xs; x <= range._xe; x++) {
		if (x < 0 || x >= _division_count)
			out_of_bounds = true;
		else
			for (int y = range._ys; y <= range._ye; y++)
				if (y < 0 || y >= _division_count)
					out_of_bounds = true;
				else {
					auto result = list_at(x, y).collider_or_null(collidable);
					if (result)
						return result;
				}
	}
	if (out_of_bounds)
		return _outside_items.collider_or_null(collidable);
	else
		return 0;
}

}

