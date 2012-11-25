/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#include "snake.h"
#include "../systemex/log.h"


namespace duality {

Snake::Snake(const Vector& startingPoint,
		const Vector& lookingAt,
		const Scalar& distancePerMove,
		const Scalar& radiansPerTurn,
		const int initialGrowth) :
		_points(),
		_head(SNAKE_MEM_SIZE-1,0),
		_tail(SNAKE_MEM_SIZE-1,0),
		_move_vector(lookingAt * distancePerMove),
		_rotation_angle(0.0f),
		_radians_per_move(radiansPerTurn),
		_translation_vector(_move_vector),
		_remaining_growth(initialGrowth)
	{
		_points[_head] = startingPoint;
	}

void Snake::move(const SteerDirection& dir) {
	switch (dir) {
	case Left:
		_rotation_angle -= _radians_per_move;
		_move_vector = Matrix(MatrixOp::rotateY, _rotation_angle) * _translation_vector;
		break;
	case Right:
		_rotation_angle +=  _radians_per_move;
		_move_vector = Matrix(MatrixOp::rotateY, _rotation_angle) * _translation_vector;
		break;
	case Forward:
		// do nothing
		break;
	}
	const auto newPoint = _points[_head] + _move_vector;
	_points[--_head] = newPoint;
	if (is_growing())
		_remaining_growth--;
	else
		-- _tail;
}

void Snake::grow(const unsigned int sizeIncrement) {
	_remaining_growth += sizeIncrement;
	if (_remaining_growth + size() >= SNAKE_MEM_SIZE)
		_remaining_growth = SNAKE_MEM_SIZE - size() -1;
}

void SnakeObject::draw(const DrawContext& gc) {
	// TODO 100 implement draw method for snake
	glPointSize(3); // pixels
	glBegin(GL_POINTS);
	for (auto index = tail_index(); index != head_index(); --index)
		glVertex3fv(points()[index].c_elems());
	glEnd();
}

SnakeObject::SnakeObject() : _left_key_down(false), _right_key_down(false) {
}

void SnakeObject::update(const UpdateContext& uc) {
	// TODO 200 implement update method for snake
	switch (uc.key_down()) {
	case SDLK_a:
		_left_key_down = true;
		break;
	case SDLK_d:
		_right_key_down = true;
		break;
	default:
		break;
	}
	switch (uc.key_up()) {
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

}

