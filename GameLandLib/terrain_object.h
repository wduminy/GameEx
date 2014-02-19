/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#pragma once
#include "game.h"
#include "game_math.h"
#include "log.h"
#include <GL/gl.h>
namespace terrain {
	using game::Vector;
	using game::Byte;
	using game::Scalar;

void render_terrain(const game::Glex& gl, GLuint array, size_t cols, size_t rows);

/** 
 A GameObject that renders a heightmap.  
 The data is provided in a Heightmap and changed to render coordinates by a Transformer 
 @tparam elemT is produced by the transformer
 @tparam heightmapT contains the data
 @tparam tranformerT transforms data coordinates to render coordinates  
 */
template <typename elemT, typename heightmapT, typename transformerT> class TerrainObject
		: public game::GameObject {
public:
	TerrainObject(heightmapT * hmap, const transformerT& transformer)
		: _hmap(hmap), _transformer(transformer), _array(0), _index_buffer(0), _program() {}

	void initialise(const game::ResourceContext & rc, const game::DrawContext& dc) override {
		const auto vert_size = _hmap->size()*3;
		std::vector<GLfloat> verts(vert_size);
		_hmap->traverse([&](int c, int r, elemT h) {
				const size_t idx = _hmap->index_of(c,r) * 3;
				const auto v = _transformer(c,r,h);
				ASSERT(idx < vert_size);
				verts[idx] = v.x();
				verts[idx + 1] = v.y();
				verts[idx + 2] = v.z();
		});
		const auto index_size = _hmap->size_traverse_triangles();
		std::vector<GLuint> indices(index_size);
		int i = 0;
		_hmap->traverse_triangles([&](int c, int r, elemT h) {
			if (c != -1) {
				const size_t idx = _hmap->index_of(c,r);
				ASSERT(idx < index_size);
				indices[i++] = static_cast<GLuint>(idx);
			} 
		});
		_program.initialise(dc,rc.load_text("terrain.vert"), rc.load_text("terrain.frag"));
		auto position = _program.attribute_location("position");
		dc.gl().glGenVertexArrays(1, &_array);
		dc.gl().glBindVertexArray(_array); 

		dc.gl().glGenBuffers(1,&_vertex_buffer);
		dc.gl().glBindBuffer(GL_ARRAY_BUFFER, _vertex_buffer);  
		dc.gl().glBufferData(GL_ARRAY_BUFFER,vert_size*sizeof(GLfloat), (void*) verts.data(),GL_STATIC_DRAW);
		dc.gl().glEnableVertexAttribArray(position);
		dc.gl().glVertexAttribPointer(position, 3, GL_FLOAT, GL_FALSE, 0, 0);

		dc.gl().glGenBuffers(1,&_index_buffer);
		dc.gl().glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _index_buffer);  
		dc.gl().glBufferData(GL_ELEMENT_ARRAY_BUFFER,index_size*sizeof(GLuint), (void*) indices.data(),GL_STATIC_DRAW);
	}

	void draw(const game::DrawContext& dc) override {
		_program.begin();
		render_terrain(dc.gl(),_array,_hmap->count_columns(), _hmap->count_rows());
		_program.end();
	}

  /**
  Draw a wire grid.  Call this for debugging your scene.
   */
	void draw_wire(const game::DrawContext& dc) {
		glPolygonMode(GL_FRONT, GL_LINE);
		glColor3f(0,1,0);
 		glEnable(GL_LINE_SMOOTH);
 		glLineWidth(1);
		render_terrain(dc.gl(),_array,_hmap->count_columns(), _hmap->count_rows());
		glColor3f(1,0,0);
    	glBegin(GL_TRIANGLE_STRIP);
 		_hmap->traverse_triangles([&](int c, int r, Byte h) {
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

  /** 
  The point of intersection at the world coordinates (x,y).
  */
 	Vector floor_at(const Scalar x, const Scalar y) const {
		Vector triangle_coords[3];
		int at = 0;
		_hmap->apply_triangle(_transformer.inverse_x(x),_transformer.inverse_y(y),
			[&] (int c, int r, Byte v) { 
				triangle_coords[at++] = _transformer(c,r,v); 
			});
		return plane_point(triangle_coords[0], triangle_coords[1], triangle_coords[2],x,y);
	}

	/** 
  The surface normal at the world coordinates (x,y).
  If (x,y) is a touches more than one triangle, the choice of
  which one is used to determine the normal is stable but arbitrary.
  The result is not normalised.
  */
 	Vector normal_at(const Scalar x, const Scalar y) const {
		Vector triangle_coords[3];
		int at = 0;
		_hmap->apply_triangle(_transformer.inverse_x(x),_transformer.inverse_y(y),
			[&] (int c, int r, Byte v) { 
				triangle_coords[at++] = _transformer(c,r,v); 
			});
		const auto a = triangle_coords[0] - triangle_coords[2];
		const auto b = triangle_coords[1] - triangle_coords[2];
		return cross_product(a, b);
	}


  /**
  Calculates the avarage of the norms arround the given heightmap coordinates (x,y).
  The result is not normalised.
  */
	Vector average_normal_at(const size_t c, const size_t r) const {
		int count = 0;
		Vector sum;
		Vector previous;
		const auto middle = _transformer(c,r,(*_hmap)(c,r));
		_hmap->fan_triangles(c,r,
			[&] (size_t cc, size_t cr, Byte v) { 
				 const auto current = _transformer(cc,cr,v); 
				 if (count > 0) {
				 		const auto a = previous - middle;
				 		const auto b = current - middle;
				 		auto n = cross_product(a,b);
					 	sum += n;
				 //		sum.normalise();
				 }
				 count++;
				 previous = current;
			});
			return sum;		
	}

	/**
	The point at the floor in the north-east corner of the terrain.
	*/
	Vector floor_north_east() const {
		return floor_at(_transformer.x(0),_transformer.y(_hmap->count_rows()-1));
	}

	/**
	The point at the floor in the south-west corner of the terrain.
	*/
	Vector floor_south_west() const {
		return floor_at(_transformer.x(_hmap->count_columns()-1),_transformer.y(0));
	}

protected:
	std::unique_ptr<heightmapT> _hmap;
	const transformerT _transformer;
private:
	GLuint _array,_index_buffer,_vertex_buffer;
	game::ShaderProgram _program;
};

}
