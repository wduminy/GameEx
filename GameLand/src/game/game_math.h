// see licence.txt

#pragma once
#include "../systemex/systemex.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <valarray>
#include <cmath>
#include <ostream>
#include <iomanip>

namespace game {
	using std::valarray;
	using std::runtime_error;
	using std::ostream;

	typedef GLfloat Scalar;
	extern const Scalar unity;
	extern const Scalar pi;   // pi is 180 degrees
	extern const Scalar piX2; // pi times two

	inline Scalar sqr(const Scalar& v) {return v * v;}

	/**
	 * Note that positive z points out of the screen - towards the operator
	 */
	class Vector {
		public:
			Vector(const Vector &source);
			Vector(const Scalar& x, const Scalar & y, const Scalar &z);
			Scalar x() const {return _data[0];}
			Scalar y() const {return _data[1];}
			Scalar z() const {return _data[2];}
			Scalar norm() const {return sqrt(sqr(x()) + sqr(y()) + sqr(z()));}
			const Scalar& operator()(const size_t& i) const {if (i > 2) return unity; else return _data[i];}
			// this exposes the internal data for use by OPENGL (use only there)
			Scalar* c_elems() const { return const_cast<Scalar*> (&_data[0]); };
		public:
			friend Vector operator+(const Vector& a, const Vector &b);
			friend Vector operator-(const Vector& a, const Vector &b);
			friend Vector operator*(const Vector& a, const Scalar &b);
			friend Scalar dot_product(const Vector& a, const Vector &b);
			friend Vector cross_product(const Vector& a, const Vector &b);
			friend ostream& operator<<(ostream& out, const Vector &m);
		private:
			Vector(const valarray<Scalar>& d);
			valarray<Scalar> _data;
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

}
