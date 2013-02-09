/**
 * Copyright 2013 Willem Duminy
 * See LICENCE.txt
 */
#include "collisions2d.h"
#include <assert.h>
namespace game {
bool BoundedBox2::in_bounds_of(const BoundedBox2& other) const {
	return overlap(left(), right(), other.left(), other.right())
			&& overlap(top(), bottom(), other.top(), other.bottom());
}

void Polygon::add(const Vector2& value) {
	_points.push_back(value);
	on_add(value);
}

void Polygon::add_relative(const Vector2& value) {
	add(_points.back() + value);
}

bool axis_overlap(const Polygon &p1, const Polygon &p2) {
	auto axlist = p1.axes();
	for (auto axis = axlist.begin(); axis != axlist.end(); axis++) {
		assert(axis->data().size() == 2);
		const Range pp1 = p1.project(*axis);
		const Range pp2 = p2.project(*axis);
		if (!pp1.overlaps(pp2))
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

bool CollidablePolygon::collides_with(const CollidablePolygon& other) const {
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
	if (p.y() > _rb.x())
		_rb.set_x(p.y());
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
	assert(axis.data().size() == 2);
	assert(_points[0].data().size() == 2);
	auto min = axis.dot(_points[0]);
	auto max = min;
	for (auto i = 1U; i < _points.size(); i++) {
		auto r = axis.dot(_points.at(i));
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
	s << "{";
	for_each(p, v._points)
	{
		s << *p;
	}
	return s;
}

CollisionManager::CollisionManager(CollisionListener::u_ptr listener) :
		_listener(std::move(listener)),
		_items() {}


bool CollisionManager::add_if_not_collide(CollidablePolygon* collidable) {
	assert(collidable != 0);
	for_each(e, _items) {
		// short circuit boolean evaluation assumed
		if ((*e)->in_bounds_of(*collidable) && (*e)->collides_with(*collidable))
			return false;
	}
	_items.push_back(collidable);
	return true;
}

void CollisionManager::remove(CollidablePolygon* collidable) {
	_items.remove(collidable);
}

}
