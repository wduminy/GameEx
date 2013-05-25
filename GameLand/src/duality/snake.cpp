/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#include "snake.h"
#include "../systemex/log.h"

namespace duality {

const unsigned char SPINE = 0;

SpinePoint::SpinePoint() :
		_topMiddle(), _bottomLeft(), _bottomRight(),
		_polygon(SPINE,Vector2::origin), _poly_is_dirty(true),
		_previous(0) {};

void SpinePoint::assign(const Vector& topMiddlePoint) {
	_topMiddle = _bottomLeft = _bottomRight = topMiddlePoint;
	_poly_is_dirty = true;
};

void SpinePoint::assign(const Vector& topMiddlePoint, const SpinePoint* prev) {
	_topMiddle = topMiddlePoint;
	Vector point_dir = topMiddlePoint - prev->topMiddle();
	auto n = point_dir.norm();
	ASSERT(n != 0);
	point_dir /= n;
	Vector perpendicular(point_dir.y(),-point_dir.x(),0);
	auto sideMovement = perpendicular * SNAKE_WIDTH_HALF;
	_bottomLeft = _topMiddle + sideMovement;
	_bottomLeft.set_z(SNAKE_FLOOR);
	_bottomRight = _topMiddle - sideMovement;
	_bottomRight.set_z(SNAKE_FLOOR);
	_poly_is_dirty = true;
	_previous = prev;
}

Snake::Snake(const Vector& startingPoint,
		const Vector& lookingAt,
		const int updatesPerGrow,
		const Scalar& distancePerMove,
		const Scalar& radiansPerTurn,
		const int initialGrowth
		) :
		_points(),
		_head(SNAKE_MEM_SIZE-1,0),
		_tail(SNAKE_MEM_SIZE-1,1),
		_move_vector(),
		_rotation_angle(0.0f),
		_radians_per_move(radiansPerTurn),
		_translation_vector(lookingAt * distancePerMove),
		_remaining_growth(0),
		_time_to_grow(updatesPerGrow),
		_previous_head(),
		_is_alive(true)
	{
		reset(startingPoint, lookingAt, initialGrowth);
	}

void Snake::move(const SteerDirection& dir) {
	if (!_is_alive)
		return;
	switch (dir) {
	case Left:
		_rotation_angle += _radians_per_move;
		_move_vector = Matrix(MatrixOp::rotateZ, _rotation_angle) * _translation_vector;
		break;
	case Right:
		_rotation_angle -=  _radians_per_move;
		_move_vector = Matrix(MatrixOp::rotateZ, _rotation_angle) * _translation_vector;
		break;
	case Forward:
		// do nothing
		break;
	}
	if (_move_vector.is_zero())
		return;
	const auto newPoint = _points[_head].topMiddle() + _move_vector;
	if (_time_to_grow.tick()) {
		if (is_growing())
			_remaining_growth--;
		else {
			before_tail_remove();
			-- _tail;
		}
		const int rem_head =  _head;
		-- _head;
		_points[_head].assign(newPoint, &_points[_previous_head]);
		on_head_added();
		ASSERT(rem_head != _previous_head);
		_previous_head = rem_head;
	} else
		_points[_head].assign(newPoint, &_points[_previous_head]);
}

void Snake::grow(const unsigned int sizeIncrement) {
	_remaining_growth += sizeIncrement;
	if (_remaining_growth + size() >= SNAKE_MEM_SIZE)
		_remaining_growth = SNAKE_MEM_SIZE - size() -1;
}

void SnakeObject::draw(const DrawContext& gc) {
    _program_p->begin();
    _program_p->arg("snakeSkin",_tex_p->index());
    // i am not sure why this call to activate is needed
    _tex_p->activate(_tex_p->index());
	//glLineWidth(3); // pixels
	glBegin(GL_TRIANGLE_STRIP);
	auto index = tail_index();
	int previous_point = index;
	--index;
	const auto &firstPoint = points()[previous_point];
	glVertex3fv(firstPoint.bottomRight().c_elems());
	for (; index != head_index(); --index) {
		const auto &p = points()[previous_point];
		const auto &q = points()[index];
		glTexCoord2d(previous_point,0);
		glVertex3fv(p.topMiddle().c_elems());
		glTexCoord2d(index,1.0);
		glVertex3fv(q.bottomRight().c_elems());
		previous_point = index;
	}
	const auto &lastPoint = points()[previous_point];
	glTexCoord2d(index,1.0);
	glVertex3fv(lastPoint.topMiddle().c_elems());
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
	auto index2 = tail_index();
	previous_point = index2;
	--index2;
	glVertex3fv(firstPoint.topMiddle().c_elems());
	for (; index2 != head_index(); --index2) {
		const auto &p = points()[previous_point];
		const auto &q = points()[index2];
		glTexCoord2d(previous_point,1.0);
		glVertex3fv(p.bottomLeft().c_elems());
		glTexCoord2d(index2,0);
		glVertex3fv(q.topMiddle().c_elems());
		previous_point = index2;
	}
	glTexCoord2d(index2,0);
	glVertex3fv(lastPoint.bottomLeft().c_elems());
	glEnd();
	_program_p->end();

}

SnakeObject::SnakeObject(CollisionManager &mgr) : SnakeWithCollision(mgr),
		_left_key_down(false),
		_right_key_down(false),
		_program_p(),
		_tex_p() {}

void SnakeObject::initialise(const ResourceContext& ctx,
		const DrawContext& draw) {
	_program_p = ctx.load_program(draw.gl(),"snake");
	_tex_p = ctx.load_texture_bmp(draw.gl(),"snakeskin.bmp",1);
}

void SnakeObject::update(const UpdateContext& uc) {
	switch (uc.input().key_down()) {
	case SDLK_a:
		_left_key_down = true;
		break;
	case SDLK_d:
		_right_key_down = true;
		break;
	default:
		break;
	}
	switch (uc.input().key_up()) {
	case SDLK_a:
		_left_key_down = false;
		break;
	case SDLK_d:
		_right_key_down = false;
		break;
	default:
		break;
	}
	SteerDirection dir;
	if (_left_key_down && !_right_key_down)
		dir = Left;
	else if (_right_key_down && !_left_key_down)
		dir = Right;
	else
		dir = Forward;
	move(dir);
}

void SnakeWithCollision::on_head_added() {
	if (_col_mgr.check_and_add(&head().polygon()) == false)
		kill();
}

void SnakeWithCollision::before_tail_remove() {
	_col_mgr.remove(&tail().polygon());
}

CollidablePolygon& SpinePoint::polygon() const {
	if (!_poly_is_dirty)
		return _polygon;
	if (is_empty() || !_previous)
		_polygon.set_start(_bottomLeft);
	else {
		Vector2 bl(_bottomLeft),
				br(_bottomRight);
		_polygon.set_start(bl);
		_polygon.add(br);
		_polygon.add(br.towards(_previous->base(),0.5));
		ASSERT(!_polygon.collides_with(_previous->polygon()));
	}
	_poly_is_dirty = false;
	return _polygon;
}

void Snake::reset(const Vector& startingPoint,
		const Vector& lookingAt,const int initialGrowth) {
	_tail.assign(1);
	_head.assign(0);
	_previous_head = 1;
	_is_alive = true;
	_rotation_angle = 0;
	_move_vector = _translation_vector;
	_remaining_growth = initialGrowth;
	_points[_tail].assign(startingPoint);
	_points[_head].assign(startingPoint + _move_vector, &_points[_tail]);
	on_reset();
}

}

