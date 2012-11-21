/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#include "Snake.h"
namespace duality {

Snake::Snake(const Vector& startingPoint, const Vector& lookingAt, const Scalar& distanceperMove, const Scalar& radiansPerTurn) :
		_points(),
		_head(MAX_SNAKE_SIZE-1,0),
		_tail(MAX_SNAKE_SIZE-1,0),
		_move_vector(lookingAt * distanceperMove),
		_rotation_angle(0.0f),
		_radians_per_move(radiansPerTurn),
		_translation_vector(_move_vector)
	{
		_points[_head] = startingPoint;
	}

void Snake::move(const SteerDirection& dir) {
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
	const auto newPoint = _points[_head] + _move_vector;
	_points[--_head] = newPoint;
}

}

