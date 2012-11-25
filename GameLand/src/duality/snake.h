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

	const size_t SNAKE_MEM_SIZE = 1000;
	enum SteerDirection : int {
		Left,
		Forward,
		Right
	};

	typedef std::array<Vector,SNAKE_MEM_SIZE> SnakeArray;

	class Snake {
	public:
		Snake(const Vector& startingPoint = Vector::origin,
				const Vector& lookingAt = Vector::north,
				const Scalar& distancePerMove = 0.02,
				const Scalar& radiansPerTurn = degrees45 / 10.0f,
				const int initialGrowth = 200);
		void move(const SteerDirection& steerDir = Forward);
		size_t size() const {return (_tail-_head);};
		void grow(const unsigned int sizeIncrement);
		bool is_growing() const {return _remaining_growth;}
		const Vector& head() const {return _points[_head];};
		const Vector& tail() const {return _points[_tail];};
		virtual ~Snake(){};
	protected:
		const RotateInt& head_index() const {return _head;};
		const RotateInt& tail_index() const {return _tail;};
		const SnakeArray& points() const {return _points;}
	private:
		SnakeArray _points;
		RotateInt _head, _tail;
		Vector _move_vector;
		Scalar _rotation_angle;
		const Scalar _radians_per_move;
		const Vector _translation_vector;
		unsigned int _remaining_growth;
	};

	class SnakeObject : public GameObject, private Snake {
	public:
		SnakeObject();
		void draw(const DrawContext& gc) override;
		void update(const UpdateContext & uc) override;
		virtual ~SnakeObject(){}
	private:
		bool _left_key_down, _right_key_down;
	};
}
