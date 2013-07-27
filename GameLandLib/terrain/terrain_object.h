/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#pragma once
#include "../game/game.h"
#include "heightmap.h"
#include "../systemex/log.h"

namespace terrain {
using namespace game;

template <typename elemT, typename heightmapT, typename transformerT> class TerrainObject
		: public GameObject {
public:
	TerrainObject(heightmapT * hmap, const transformerT& transformer)
		: _hmap(hmap), _transformer(transformer), _buffer(0) {}

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
		glPolygonMode(GL_FRONT, GL_FILL);
		glColor3b(100,100,100);
		draw_strips(dc);
		glPolygonMode(GL_FRONT, GL_LINE);
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(2);
		glColor3b(50,0,0);
		draw_strips(dc);
	}
protected:
	void draw_strips(const Glex& dc) {
		dc.glBindBuffer(GL_ARRAY_BUFFER,_buffer);
		dc.glEnableVertexAttribArray(0);
		dc.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		// draw column per column
		const auto cols = _hmap->count_columns();
		const auto rows = _hmap->count_rows();
		const auto prims = rows * 2;
		for (size_t i = 0; i < cols;i++)
			glDrawArrays(GL_TRIANGLE_STRIP,i*prims,prims);

		dc.glDisableVertexAttribArray(0);
	}
	std::unique_ptr<heightmapT> _hmap;
private:
	transformerT _transformer;
	GLuint _buffer;
};

}
