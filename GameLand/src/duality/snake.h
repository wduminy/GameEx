/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#pragma once
#include "../game/game.h"
#include "../game/game_math.h"
#include <array>


namespace duality {
	using namespace game;

	const size_t MAX_SNAKE_SIZE = 100;
	enum SteerDirection : int {
		Left,
		Forward,
		Right
	};

	class Snake {
	public:
		Snake(const Vector& startingPoint, const Vector& lookingAt,
				const Scalar& distancePerMove = 1.0, const Scalar& radiansPerTurn = degrees45);
		void move(const SteerDirection& steerDir = Forward);
		int size() const {return (_tail-_head);};
		const Vector& head() const {return _points[_head];};
		const Vector& tail() const {return _points[_tail];};
	private:
		std::array<Vector,MAX_SNAKE_SIZE> _points;
		RotateInt _head, _tail;
		Vector _move_vector;
		Scalar _rotation_angle;
		const Scalar _radians_per_move;
		const Vector _translation_vector;
	};

}
