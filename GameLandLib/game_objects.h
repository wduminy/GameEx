/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#include "game_base.h"
#include "game_math.h"
#include <array>
#include <vector>

#pragma once
namespace game {
	using std::vector;

  // triangles are specified anti-clockwise
	class TriangleStrip {
			PREVENT_COPY(TriangleStrip);
		public:
			TriangleStrip(const int numberOfTriangles);
			void push_back(const GLfloat& x, const GLfloat& y, const GLfloat& z);
			void push_back3f(const GLfloat* xyz);
			void push_back(const vector<const GLfloat*> &points);
			void push_back_reverse(const vector<const GLfloat*> &points);
			void push_back(const Vector2 &v) {push_back(v.x(),v.y(),0);}
			void push_back(const Vector &v) {push_back(v.x(),v.y(),v.z());}
			void reset() const {_it = _verts.get();}
			bool at_end() const {return _it >= _ins;}
			void draw_step() const {glVertex3fv(_it);_it+=3;}
			void draw() const;
		private:
			std::unique_ptr<GLfloat []> _verts;
			GLfloat * _ins;
			GLfloat * _end;
			mutable GLfloat * _it;
	};

	/**
	 * A Panel is a two dimensional surface drawn using screen coordinates.  
	 * This coordinate system is such that (-1,1) is left top, (0,0) is the middle of the screen
	 * and (1,-1) is right bottom.
	 * In your media (see ResourceContext) must include a shader named
	 * "panel" (vert and frag).
	 * After construction, you typically draw on the surface() and then call
	 * update_buffer() to copy the surface to the OpenGL buffer.
	 */
	class Panel : public GameObject {
	public:
		/**
		 * \param x and y is the left top position of the panel (using Panel coordinates)
		 * \param width and height is the width and height (also in Panel coordinates)
		 * \param pixelW and pixelH is the number of pixels in the SDL_Surface
		 */
		Panel(const Scalar x, Scalar y, const Scalar width, const Scalar height, int pixelW, int pixelH);
		/** The underlying Surface -- in normal memory.  After you make updates
		 * you must call update_buffer() before the rendering changes  
		 */
		Surface& surface() {return *_surface;}
		/** Convenient accessor for surface() */
		operator SDL_Surface& () {return *_surface;}
		void initialise(const ResourceContext &rc, const DrawContext& dc) override;
		void draw(const DrawContext &dc) override;
		/** Copy the surface() from normal memory to GPU memory. */
		void update_buffer() {_tex->copy_from_2d(*_surface);}
	private:
		TriangleStrip _rectangle;
		ShaderProgram::u_ptr _program;
		Texture::u_ptr _tex;
	  Surface::u_ptr _surface;
		GLfloat _left;
		GLfloat _top;
		GLfloat _width;
		GLfloat _height;
		int _pixelW;
		int _pixelH;
	};

	class SphereCamera : public GameObject {
		public:
			SphereCamera(int drawOrder, double circleZ = 5, double circleRadius = 5, const Vector& location = Vector::origin);
			void update(const UpdateContext& ctx);
			void draw(const DrawContext& ctx);
			Vector& location() { return _location; }
		private:
			double _circleZ;
			const double _circleRadius;
			double _theta;
			double _delta;
			Vector _location;
	};


}



