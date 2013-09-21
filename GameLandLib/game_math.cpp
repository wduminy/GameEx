/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#include "game_math.h"
#include "log.h"

namespace game {
	const Vector Vector::origin(0,0,0);
	const Vector Vector::north(0,1,0);
	const Vector Vector::south = Vector::north * -1.0f;
	const Vector Vector::west(-1,0,0);
	const Vector Vector::east = Vector::west * -1.0f;
	const Vector Vector::up(0,0,1);
	const Vector Vector::down = Vector::up * -1.0f;
	const Vector2 Vector2::origin(zero,zero);
	const Scalar unity = 1.0f;
	const Scalar zero = 0.0f;
	const auto degrees90 = asin(1);
	const Scalar pi = degrees90 * 2.0f;
	const Scalar piX2 = degrees90 * 2.0f;
	const Scalar degrees45 = degrees90 / 2.0f;

	Vector2 operator-(const Vector2& a, const Vector2 &b) {
			return Vector2(a.data() - b.data());
	}

	Vector2 operator+(const Vector2& a, const Vector2 &b) {
		return Vector2(a.data() + b.data());
	}

	Vector2 operator*(const Vector2& a, const Scalar s) {
		return Vector2(a.data() * s);
	}

	Vector2 operator/(const Vector2& a, const Scalar s) {
		return Vector2(a.data() / s);
	}

	ostream& operator<<(ostream& out, const ScalarValueArray &m) {
			out << "(" ;
			for (size_t i = 0U; i < m.data().size(); i++ ) {
				if (i != 0U) out << ",";
				out.width(6);
				out.precision(3);
				out << m._data[i];
			}
			out << ")";
			return out;
	}

	Vector::Vector(const Scalar& x, const Scalar & y, const Scalar &z) : Vector2(3) {
		set_x(x);
		set_y(y);
		set_z(z);
	}

	void ScalarValueArray::normalise() {
		const auto n = norm();
		if (n == 0)
			throw std::runtime_error("cannot normalise - will cause a division by zero");
		_data /= n;
	}

	bool ScalarValueArray::is_zero() const {
		for (size_t i = 0U; i <_data.size(); i++ )
			if (!scalar_equals(_data[i],zero))
				return false;
		return true;
	}

	bool ScalarValueArray::operator ==(const ScalarValueArray& other) const {
		for (size_t i = 0U; i <_data.size(); i++ )
			if (!scalar_equals(_data[i], other._data[i]))
					return false;
		return true;
	}


	Vector::Vector(const valarray<Scalar>& d): Vector2(d) {
		assert(d.size() == 3);
	}

	Vector::Vector(const Vector& source) :  Vector(source.data()) { }

	Vector operator+(const Vector& a, const Vector &b) {
		return Vector(a.data() + b.data());
	}

	Vector operator-(const Vector& a, const Vector &b) {
		return Vector(a.data() - b.data());
	}

	Vector operator*(const Vector& a, const Scalar b) {
		return Vector(a.data() * b);
	}

	Vector operator/(const Vector& a, const Scalar b) {
		return Vector(a.data() / b);
	}

	Scalar dot(const Vector& a, const Vector &b) {
		return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
	}

	Vector cross_product(const Vector& a, const Vector &b) {
		return Vector(a.y() * b.z() - b.y()*a.z(), a.z() * b.x() - b.z() * a.x(), a.x() * b.y() - b.x() * a.y());
	}

	/** Determines the point in a plane that intersects with a vertical line.
	@param a,b,c the points that defines the plane.
	@param x,y the position through which the vertical line goes.
	*/
	Vector plane_point(const Vector& a, const Vector &b, const Vector &c, const Scalar x, const Scalar y) {
		const auto e1 = ( ( b.x()-a.x()) * c.z()+( a.z()-b.z()) * c.x()+a.x() * b.z()-a.z() * b.x());
		const auto e2 = ( ( a.y()-b.y()) * c.z()+( b.z()-a.z()) * c.y()-a.y() * b.z()+a.z() * b.y());
		const auto e3 = ( a.x() * b.y()-a.y() * b.x()) * c.z()+( a.z() * b.x()-a.x() * b.z()) * c.y()+( a.y() * b.z()-a.z() * b.y()) * c.x();
		const auto e4 = ( b.x()-a.x()) * c.y()+( a.y()-b.y()) * c.x()+a.x() * b.y()-a.y() * b.x();
		ENSURE(e4 != 0, "a,b and c do not form a triangle");
		const auto z = (e1 * y + e2 * x +e3)/e4; 
		return Vector(x,y,z);
	}

	const size_t diagonals[] = {0,5,10,15};
	valarray<size_t> diagonal(diagonals,4);

	const MatrixOp::Enum MatrixOp::translate = new MatrixOp();
	const MatrixOp::Enum MatrixOp::scale = new MatrixOp();
	const MatrixOp::Enum MatrixOp::rotateX = new MatrixOp();
	const MatrixOp::Enum MatrixOp::rotateY = new MatrixOp();
	const MatrixOp::Enum MatrixOp::rotateZ = new MatrixOp();

	Matrix::Matrix() : _data(16) {
		_data[diagonal] = 1.0f;
	}

	Matrix::Matrix(const Matrix& source) : _data(source._data) {}

	Matrix::Matrix(const MatrixOp::Enum op,const Vector &v) : _data(16) {
		if (op == MatrixOp::translate) {
			_data[diagonal] = 1.0f;
			_data[12] = v.x();
			_data[13] = v.y();
			_data[14] = v.z();
		} else if (op == MatrixOp::scale) {
			_data[0] = v.x();
			_data[5] = v.y();
			_data[10] = v.z();
			_data[15] = 1;
		} else
			throw runtime_error("op not supported by this constructor");
	}

	Matrix::Matrix(const MatrixOp::Enum op,const Scalar &angle) : _data(16) {
		_data[15] = 1;
		const Scalar sina = sin(angle);
		const Scalar cosa = cos(angle);
		if (op == MatrixOp::rotateX) {
			_data[5] = cosa;
			_data[6] = sina;
			_data[9] = -sina;
			_data[10] = cosa;
			_data[0] = 1;
		} else if (op == MatrixOp::rotateY) {
			_data[0] = cosa;
			_data[2] = sina;
			_data[8] = -sina;
			_data[10] = cosa;
			_data[5] = 1;
		} else if (op == MatrixOp::rotateZ) {
			_data[0] = cosa;
			_data[1] = sina;
			_data[4] = -sina;
			_data[5] = cosa;
			_data[10] = 1;
		} else
			throw runtime_error("op not supported by this constructor");
	}

	Matrix::Matrix(const valarray<Scalar>& d): _data(d) {}

	Matrix operator+(const Matrix& a, const Matrix &b) {
		return Matrix(a._data + b._data);
	}

	Matrix operator-(const Matrix& a, const Matrix &b) {
		return Matrix(a._data - b._data);
	}

	Matrix operator*(const Matrix& a, const Scalar& b) {
		return Matrix(a._data * b);
	}

	Matrix operator*(const Matrix& a, const Matrix &b) {
		Matrix result;
		Scalar sum;
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
					sum = 0;
					for (int k = 0; k < 4; k++)
						sum += a(row,k) * b(k,col);
					result._data[col*4+row] = sum;
			}
		}
		return result;
	}

	Vector operator*(const Matrix& a, const Vector &b) {
		Scalar result[4] = {0,0,0};
		Scalar sum;
		for (int row = 0; row < 4; row++) {
			sum = 0;
			for (int k = 0; k < 4; k++)
					sum += a(row,k) * b(k);
			result[row] = sum;
		}
		return Vector(result[0],result[1],result[2]);
	}

	ostream& operator<<(ostream& out, const Matrix &m) {
		for (size_t row = 0; row < 4;row++) {
			out << "| ";
			for (size_t col = 0; col < 4;col++) {
				out << std::setw(5) << m(row,col) << " ";
			}
			out << "|"  << std::endl;
		}
		return out;
	}

	RotateInt::RotateInt(const int maxValue, const int value) : _max_value(maxValue), _value(value) {
		ASSERT((value >= 0) && (value <= _max_value));
	}

	RotateInt& RotateInt::operator ++() {
		_value++;
		if (_value == _max_value)
			_value = 0;
		return *this;
	}

	RotateInt& RotateInt::operator --() {
		--_value;
		if (_value == -1)
			_value = _max_value;
		return *this;
	}

	int operator -(const RotateInt& a, const RotateInt& b) {
		ASSERT(a.max() == b.max());
		const auto ia = (int) a;
		const auto ib = (int) b;
		if (ia >= ib)
			return ia - ib;
		else
			return (b.max() - ib + 1) + ia;
	}

}
