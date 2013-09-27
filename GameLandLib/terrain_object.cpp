/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#include "terrain_object.h"
#include <GL/gl.h>
 
namespace terrain {
	void render_strips(const game::Glex& gl, GLuint array, size_t cols, size_t rows) {
		gl.glBindVertexArray(array);
		// draw column per column
		const auto prims = rows * 2 * sizeof(GLuint);
		for (size_t i = 0; i < cols-1;i++)
			glDrawElements(GL_TRIANGLE_STRIP,rows*2,GL_UNSIGNED_INT,(void*)(i*prims));
		gl.glBindVertexArray(0);

	}
}
void terrain::render_terrain(const game::Glex& gl, GLuint array, size_t cols, size_t rows) {
	glPolygonMode(GL_FRONT, GL_FILL);
	render_strips(gl,array,cols, rows);
}
