#pragma once
#include <array>
#include <functional>
#include "..\game\game.h"

namespace terrain {
using namespace std;
using game::Scalar;
using game::Vector;


template <typename elemT, size_t m_columns, size_t n_rows > class Heightmap {
public:
	typedef std::array<elemT, n_rows * m_columns> container_t;

	Heightmap(const elemT& zero_value) : _elems() {_elems.fill(zero_value);}
	const container_t& elems() const {return _elems;}
	container_t& elems() {return _elems;}
	const elemT& operator() (size_t c, size_t r) const {
		check_range(c,r); return _elems.at(r * n_rows + c);}
	elemT& operator() (size_t c, size_t r) {
		check_range(c,r);return _elems.at(r * n_rows + c);}
	size_t count_rows() const {return n_rows; }
	size_t count_columns() const {return m_columns; }
	size_t size_traverse_triangles() const {return m_columns * n_rows * 2;}
	virtual ~Heightmap() {};
	void traverse(std::function<void (int c, int r, elemT h)> applyF) const {
		for (auto c = 0U; c < m_columns; c++)
			for (auto r = 0U; r < n_rows; r++)
				applyF(c,r,_elems.at(r * n_rows + c));
	}
	void traverse_triangles(std::function<void (int c, int r, elemT h)> applyF) const {
		for (auto c = 0U; c < m_columns-1; c++) {
			auto nc = c + 1;
			for (auto r = 0U; r < n_rows; r++) {
				applyF(nc,r,_elems.at(r * n_rows + nc));
				applyF(c,r,_elems.at(r * n_rows + c));
			}
			applyF(-1,-1,0); // signal end of triangle strip
		}
	}

protected:
	container_t _elems;
	void check_range(size_t c, size_t r) const {
		if (c < 0 || c >= m_columns) throw std::out_of_range("c is invalid");
		if (r < 0 || r >= n_rows) throw std::out_of_range("r is invalid");
	}
};

void read_heightmap(const string& filename,size_t buffer_size, game::Byte * buffer);
void write_heightmap(const string& filename,int width, int height, game::Byte * buffer);

template <size_t m_columns, size_t n_rows > class HeightmapWithByte
	: public Heightmap<game::Byte, m_columns,n_rows>{
public:
	HeightmapWithByte() : Heightmap<game::Byte,m_columns,n_rows>(0) {}
	void read_from_bmp(const string& bmp_filename) {
		read_heightmap(bmp_filename,
				Heightmap<game::Byte,m_columns,n_rows>::elems().size(),
				Heightmap<game::Byte,m_columns,n_rows>::elems().data());
	}
	void write_to_bmp(const string& bmp_filename) {
		write_heightmap(bmp_filename,
				m_columns,n_rows,
				Heightmap<game::Byte,m_columns,n_rows>::elems().data());
	}
};

template <typename elemT> class Transformer {
public:
	Transformer(const Scalar& square_length, const Scalar& height_scale)
		: _square_length(square_length), _height_scale(height_scale) {}
	Scalar x(int c) const { return c * _square_length; }
	Scalar y(int r) const { return r * _square_length * (-1); }
	Scalar z(int h) const { return h * _height_scale; }
	Vector operator()(int c, int r, elemT h) const {
		return Vector(x(c),y(r),z(h));
	}
	virtual ~Transformer() {}
private:
	const Scalar _square_length;
	const Scalar _height_scale;
};

class TransformerByte : public Transformer<game::Byte> {
public:
	TransformerByte(const Scalar& square_length,
			const Scalar& height_min, const Scalar& height_max)
	:Transformer<game::Byte>(square_length, (height_max - height_min)/255){}
};

}
