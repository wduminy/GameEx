#pragma once
#include <array>
#include <functional>
#include "game_base.h"
#include "game_math.h"

namespace terrain {
using namespace std;
using game::Scalar;
using game::Vector;


template <typename elemT, size_t m_columns, size_t n_rows > class Heightmap {
public:
	typedef std::array<elemT, n_rows * m_columns> container_t;
	typedef std::function<void (int c, int r, elemT h)> triangle_fun_t;

	Heightmap(const elemT& zero_value) : _elems() {_elems.fill(zero_value);}
	const container_t& elems() const {return _elems;}
	container_t& elems() {return _elems;}
	const elemT& operator() (size_t c, size_t r) const {
		check_range(c,r); return _elems.at(r * n_rows + c);}	elemT& operator() (size_t c, size_t r) {
		check_range(c,r);return _elems.at(r * n_rows + c);}
	size_t count_rows() const {return n_rows; }
	size_t count_columns() const {return m_columns; }
	size_t size_traverse_triangles() const {return m_columns * n_rows * 2;}
	elemT max_height() const {
		auto result = _elems.front();
		for_each(e, _elems) { if (*e > result) result = *e; }
		return result;
	}
	elemT min_height() const {
		auto result = _elems.front();
		for_each(e, _elems) { if (*e < result) result = *e; }
		return result;
	}
	virtual ~Heightmap() {};
	/** Calls a function for each vertex in the heightmap.
	The order is (0,0) (0,1) etc. */
	void traverse(triangle_fun_t applyF) const {
		for (auto c = 0U; c < m_columns; c++)
			for (auto r = 0U; r < n_rows; r++)
				applyF(c,r,_elems.at(r * n_rows + c));
	}
	/** Calls a function that helps with the rendering of a triangle strip.
	@param applyF the function that is called. applyF(-1,-1,0) indicates the end of a strip.
	*/  
	void traverse_triangles(triangle_fun_t applyF) const {
		for (auto c = 0U; c < m_columns-1; c++) {
			auto nc = c + 1;
			for (auto r = 0U; r < n_rows; r++) {
				applyF(nc,r,_elems.at(r * n_rows + nc));
				applyF(c,r,_elems.at(r * n_rows + c));
			}
			applyF(-1,-1,0); // signal end of triangle strip
		}
	}

  /** Calls a function for each vertex in the triangle that contains given coordinates.
  @param c,r the coordinates somewhere in the range (0..m_columns, 0...n_rows)
  @param applyF the function that will be called three times */
	void apply_triangle(const Scalar& r, const Scalar& c, triangle_fun_t applyF) {
		const int fc = (int) floor(c);
		const int fr = (int) floor(r);
		const int rc = (int) round(c);
		if (fc == rc) {
			applyF(fc+1,fr,_elems.at(fr * n_rows + fc+1));
			applyF(fc,fr,_elems.at(fr * n_rows + fc));
			applyF(fc,fr+1,_elems.at((fr+1) * n_rows + fc));
		} else {
			applyF(fc,fr,_elems.at(fr * n_rows + fc));
			applyF(fc+1,fr+1,_elems.at((fr+1) * n_rows + fc+1));
			applyF(fc,fr+1,_elems.at((fr+1) * n_rows + fc));			
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

/** 
 * A heightmap that has a Byte as element type. 
 */
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
	/** Adjusts the heights so that the heighest point is 255 and the
	 * lowest point is 0.
	 */
	void normalise() {
		const game::Byte min = Heightmap<game::Byte,m_columns,n_rows>::min_height();
		const game::Byte max = Heightmap<game::Byte,m_columns,n_rows>::max_height();
		const float offset = -min;
		const float scale = 255.0f / (max - min);
		auto &elems = Heightmap<game::Byte,m_columns,n_rows>::elems();
		for_each(e,elems) {*e = (*e + offset) * scale;}
	}
};

/**
 A functor that maps elements to a Vector.
 @ tparam elemT used as input
 */
template <typename elemT> class Transformer {
public:
	Transformer(const Scalar square_length, const Scalar height_scale, const Scalar height_min = 0)
		: _square_length(square_length), _height_scale(height_scale), _height_min(height_min) {}
	Scalar x(int c) const { return c * _square_length; }
	Scalar y(int r) const { return r * _square_length * (-1); }
	Scalar z(int h) const { return h * _height_scale + _height_min; }
	Vector operator()(int c, int r, elemT h) const {
		return Vector(x(c),y(r),z(h));
	}
	virtual ~Transformer() {}
private:
	const Scalar _square_length;
	const Scalar _height_scale;
	const Scalar _height_min;
};

/** A Transformer with Byte as input */
class TransformerByte : public Transformer<game::Byte> {
public:
	TransformerByte(const Scalar square_length,
			const Scalar height_min, const Scalar height_max)
	:Transformer<game::Byte>(square_length, (height_max - height_min)/255, height_min){}
};

}
