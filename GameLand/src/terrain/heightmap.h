#pragma once
#include <array>
#include "../game/sdl_objects.h"

namespace terrain {
using namespace std;

template <typename elemT, size_t m_columns, size_t n_rows > class Heightmap {
public:
	Heightmap(const elemT& zero_value) : _elems() {_elems.fill(zero_value);}
	typedef std::array<elemT, n_rows * m_columns> container_t;
	const container_t& elems() const {return _elems;}
	container_t& elems() {return _elems;}
	const elemT& operator() (size_t c, size_t r) const {
		check_range(c,r); return _elems.at(r * n_rows + c);}
	elemT& operator() (size_t c, size_t r) {
		check_range(c,r);return _elems.at(r * n_rows + c);}
	virtual ~Heightmap() {};
protected:
	container_t _elems;
	void check_range(size_t c, size_t r) const {
		if (c < 0 || c >= m_columns) throw std::out_of_range("c is invalid");
		if (r < 0 || r >= n_rows) throw std::out_of_range("r is invalid");
	}
};

void read_heightmap(const string& filename,size_t buffer_size, unsigned char * buffer);
void write_heightmap(const string& filename,int width, int height, unsigned char * buffer);

template <size_t m_columns, size_t n_rows > class HeightmapWithByte
	: public Heightmap<unsigned char, m_columns,n_rows>{
public:
	HeightmapWithByte() : Heightmap<unsigned char,m_columns,n_rows>(0) {}
	void read_from_bmp(const string& bmp_filename) {
		read_heightmap(bmp_filename,
				Heightmap<unsigned char,m_columns,n_rows>::elems().size(),
				Heightmap<unsigned char,m_columns,n_rows>::elems().data());
	}
	void write_to_bmp(const string& bmp_filename) {
		write_heightmap(bmp_filename,
				m_columns,n_rows,
				Heightmap<unsigned char,m_columns,n_rows>::elems().data());
	}
};

}
