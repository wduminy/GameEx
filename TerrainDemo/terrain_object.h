/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#pragma once
#include <game/game.h>
#include <systemex/log.h>

namespace terrain {
using namespace game;
void render_terrain(const game::Glex& gl, GLuint buffer, size_t cols, size_t rows);

template <typename elemT, typename heightmapT, typename transformerT> class TerrainObject
		: public GameObject {
public:
	TerrainObject(heightmapT * hmap, const transformerT& transformer)
		: _hmap(hmap), _transformer(transformer), _buffer(0), _program() {}

	void initialise(const ResourceContext & rc, const DrawContext& dc) override {
		dc.gl().glGenBuffers(1,&_buffer);
		const auto b_size = _hmap->size_traverse_triangles()*3;
		vector<GLfloat> verts(b_size);
		int i = 0;
		_hmap->traverse_triangles([&](int c, int r, elemT h) {
			if (c != -1) {
				const Vector v = _transformer(c,r,h);
				verts[i++] = v.x();
				verts[i++] = v.y();
				verts[i++] = v.z();
			}
		});
		dc.gl().glBindBuffer(GL_ARRAY_BUFFER, _buffer);
		dc.gl().glBufferData(GL_ARRAY_BUFFER,b_size*sizeof(GLfloat), (void*) verts.data(),GL_STATIC_DRAW);
		dc.gl().glBindBuffer(GL_ARRAY_BUFFER,0);
	}

	void draw(const DrawContext& dc) override {
		render_terrain(dc.gl(),_buffer,_hmap->count_columns(), _hmap->count_rows());
	}
protected:
	std::unique_ptr<heightmapT> _hmap;
private:
	transformerT _transformer;
	GLuint _buffer;
	ShaderProgram _program;
};

}
