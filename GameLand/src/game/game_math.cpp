// see licence.txt

#include "game_math.h"
#include "../systemex/log.h"

namespace game {
	const Scalar unity = 1.0f;
	const Scalar pi = asin(1) * 2.0f;
	const Scalar piX2 = pi * 2.0f;

	Vector::Vector(const Scalar& x, const Scalar & y, const Scalar &z) : _data(3) {
		_data[0] = x;
		_data[1] = y;
		_data[2] = z;
	}

	Vector::Vector(const valarray<Scalar>& d): _data(d) {
	}

	Vector::Vector(const Vector& source) : _data(source._data) {
	}

	Vector operator+(const Vector& a, const Vector &b) {
		return Vector(a._data + b._data);
	}

	Vector operator-(const Vector& a, const Vector &b) {
		return Vector(a._data - b._data);
	}

	Vector operator*(const Vector& a, const Scalar& b) {
		return Vector(a._data * b);
	}

	Scalar dot_product(const Vector& a, const Vector &b) {
		return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
	}

	Vector cross_product(const Vector& a, const Vector &b) {
		return Vector(a.y() * b.z() - b.y()*a.z(), a.z() * b.x() - b.z() * a.x(), a.x() * b.y() - b.x() * a.y());
	}

	ostream& operator<<(ostream& out, const Vector &m) {
			out << "| ";
			for (size_t k = 0; k < 4;k++)
				out << std::setw(5) << m(k) << " ";
			out << "| ";
			return out;
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
		Scalar result[3] = {0,0,0};
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
}
