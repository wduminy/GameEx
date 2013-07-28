/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#include "terrain_object.h"
namespace terrain {
	void render_strips(const game::Glex& gl, GLuint buffer, size_t cols, size_t rows) {
		gl.glBindBuffer(GL_ARRAY_BUFFER,buffer);
		gl.glEnableVertexAttribArray(0);
		gl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		// draw column per column
		const auto prims = rows * 2;
		for (size_t i = 0; i < cols;i++)
			glDrawArrays(GL_TRIANGLE_STRIP,i*prims,prims);

		gl.glDisableVertexAttribArray(0);
	}
}
void terrain::render_terrain(const game::Glex& gl, GLuint buffer, size_t cols, size_t rows) {
	glPolygonMode(GL_FRONT, GL_FILL);
	glColor3b(100,100,100);
	render_strips(gl,buffer,cols, rows);
	glPolygonMode(GL_FRONT, GL_LINE);
	glEnable(GL_LINE_SMOOTH);
	glLineWidth(2);
	glColor3b(50,0,0);
	render_strips(gl,buffer,cols, rows);
}
