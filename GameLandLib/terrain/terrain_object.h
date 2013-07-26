/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#pragma once
#include "../game/game.h"

namespace terrain {
using namespace game;

template <typename elemT, typename heightmapT, typename transformerT> class TerrainObject
		: public GameObject {
public:
	TerrainObject(heightmapT * hmap, const transformerT& transformer)
		: _hmap(hmap), _transformer(transformer) {}
	void draw(const DrawContext& gc) override {
		glPolygonMode(GL_FRONT, GL_FILL);
		glColor3b(100,100,100);
		draw_strips();
		glPolygonMode(GL_FRONT, GL_LINE);
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(2);
		glColor3b(50,0,0);
		draw_strips();
	}
protected:
	void draw_strips() {
		glBegin(GL_TRIANGLE_STRIP);
		_hmap->traverse_triangles([&](int c, int r, elemT h) {
			if (c == -1) {
				glEnd();
				glBegin(GL_TRIANGLE_STRIP);
			} else {
				const Vector v = _transformer(c,r,h);
				glVertex3f(v.x(),v.y(),v.z());
			}
		});
		glEnd();
	}
	std::unique_ptr<heightmapT> _hmap;
private:
	transformerT _transformer;
};

}
