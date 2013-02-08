/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#pragma once
#include "../systemex/systemex.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <valarray>
#include <cmath>
#include <ostream>
#include <iomanip>
#include <array>
#include <assert.h>
namespace game {
	using std::valarray;
	using std::runtime_error;
	using std::ostream;
	using std::array;

	typedef GLfloat Scalar;
	extern const Scalar zero;
	extern const Scalar unity;
	extern const Scalar pi;   // pi is 180 degrees
	extern const Scalar piX2; // pi times two
	extern const Scalar degrees45;

	inline Scalar sqr(const Scalar& v) {return v * v;}
	inline bool overlap(const Scalar& a1, const Scalar& a2, const Scalar& b1, const Scalar& b2) {
		return (b1 <= a2 && b1 >= a1)
				|| (b2 <= a2 && b2 >= a1)
				|| (b1 <= a1 && b2 >= a2);
	}
	inline bool scalar_equals(const Scalar& a, const Scalar& b) {return fabs(a-b) < 0.00001f;}

	class Range {
	public:
		Range(const Scalar from, const Scalar to) : _from(from), _to(to) {};
		Scalar from() const { return _from; }
		Scalar to() const {return _to; }
		bool overlaps(const Range& v) const { return overlap(_from,_to,v._from,v._to); }
	private:
		Scalar _from, _to;
	};

	class ScalarValueArray {
	public:
		Scalar operator()(const size_t& i) const {return get(i);}
		// this exposes the internal data for use by OPENGL (use only there)
		Scalar* c_elems() const { return const_cast<Scalar*> (&_data[0]); };
	 	virtual ~ScalarValueArray(){}
		void operator /= (const Scalar& v) {_data /= v;}
		virtual Scalar norm() const = 0;
		void normalise();
		const valarray<Scalar> & data() const {return _data;}
	protected:
		 explicit ScalarValueArray(const valarray<Scalar>& source) : _data(source) {}
		 explicit ScalarValueArray(size_t size) : _data(size) {}
		 void set(size_t i, Scalar v) {_data[i] = v;};
		 Scalar get(size_t i) const {assert(i < _data.size()); return _data[i];}
	private:
	 	 valarray<Scalar> _data;
	};

	class Vector2 : public ScalarValueArray  {
		public:
			Vector2(Scalar x, Scalar y) : ScalarValueArray(2) {set(0,x); set(1,y);}
			Vector2() : Vector2(zero,zero) {}
			Scalar x() const {return get(0);}
			Scalar y() const {return get(1);}
			void set_x(Scalar v) {set(0,v);}
			void set_y(Scalar v) {set(1,v);}
			Vector2 perpendicular() const {return Vector2(-y(),x());}
			Scalar dot(const Vector2 &v) const {return x() * v.x() + y() * v.y();}
			Scalar norm() const {return sqrt(sqr(x()) + sqr(y()));}
		protected:
			explicit Vector2(size_t s) : ScalarValueArray(s) {};
			explicit Vector2(const valarray<Scalar>& source) : ScalarValueArray(source) {assert(data().size() > 1);};
		public:
		 	friend Vector2 operator+(const Vector2& a, const Vector2 &b);
		 	friend Vector2 operator-(const Vector2& a, const Vector2 &b);
		 	friend Vector2 operator/(const Vector2& a, const Scalar s);
		 	friend Vector2 operator*(const Vector2& a, const Scalar s);
		 	friend ostream& operator<<(ostream& out, const Vector2 &m);
		 	static const Vector2 origin;
	};


	/**
	 * Coordinate system: x - increase to west, y increases north, z increases up you
	 * Vector objects has 4 elements
	 */
	class Vector : public Vector2 {
		public:
			Vector(const Vector &source);
			Vector(const Scalar& x = zero, const Scalar & y = zero, const Scalar &z = zero);
			Scalar z() const {return get(2);}
			void set_z(Scalar v) {set(2,v);}
			Scalar norm() const override {return sqrt(sqr(x()) + sqr(y()) + sqr(z()));}
		public:
			friend Vector operator+(const Vector& a, const Vector &b);
			friend Vector operator-(const Vector& a, const Vector &b);
			friend Vector operator*(const Vector& a, const Scalar &b);
			friend Scalar dot(const Vector& a, const Vector &b);
			friend Vector cross_product(const Vector& a, const Vector &b);
			friend ostream& operator<<(ostream& out, const Vector &m);
			static const Vector origin;
			static const Vector north;
			static const Vector west;
			static const Vector east;
			static const Vector south;
			static const Vector up;
			static const Vector down;
			bool operator==(const Vector& other) const;
			bool operator!=(const Vector& other) const;
			virtual ~Vector(){}
		private:
			Vector(const valarray<Scalar>& d);
	};

	class MatrixOp {
			PREVENT_COPY(MatrixOp);
		public:
			typedef MatrixOp * const Enum;
			static const MatrixOp::Enum translate;
			static const MatrixOp::Enum scale;
			static const MatrixOp::Enum rotateX;
			static const MatrixOp::Enum rotateY;
			static const MatrixOp::Enum rotateZ;
		private:
			MatrixOp() {}
	};



	// a column ordered matrix
	class Matrix {
		public:
			// constructs an identity matrix
			Matrix();
			Matrix(const Matrix &source);
			Matrix(const MatrixOp::Enum op,const Vector &v);
			Matrix(const MatrixOp::Enum op,const Scalar& angle);
			// this exposes the internal data for use by OPENGL (use only there)
			Scalar* c_elems() { return &_data[0]; };
			// row and column os zero offset
			const Scalar& operator()(const size_t row, const size_t col) const {return _data[col*4+row];}
		public:
			friend Matrix operator+(const Matrix& a, const Matrix &b);
			friend Matrix operator-(const Matrix& a, const Matrix &b);
			friend Matrix operator*(const Matrix& a, const Scalar &b);
			friend Matrix operator*(const Matrix& a, const Matrix &b);
			friend Vector operator*(const Matrix& a, const Vector &b);
			friend ostream& operator<<(ostream& out, const Matrix &m);
		private:
			Matrix(const valarray<Scalar>& d);
			valarray<Scalar> _data;
	};

	/**
	 * An resetable integer that counts down from a starting value to zero
	 */
	class CountDownCounter {
	public:
		CountDownCounter(int startValue) : _start_value(startValue), _value(_start_value) {}
		bool count() {
			if (--_value == 0)
				_value = _start_value;
			return _value == _start_value;
		}
	private:
		const int _start_value;
		int _value;
	};
	/**
	 * An integer that 'rotates' from 0 to maxValue
	 */
	class RotateInt {
	public:
		RotateInt(const int maxValue, const int value);
		RotateInt& operator++();
		RotateInt& operator--();
		operator int() const {return _value;}
		int max() const {return _max_value;}
		bool operator != (RotateInt& other) const {return _value != other._value;};
	private:
		const int _max_value;
		int _value;
	};

	int operator-(const RotateInt& a, const RotateInt& b);
}
