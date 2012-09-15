/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#include "game.h"
#include <array>
#pragma once
namespace game {

  // triangles are specified anti-clockwise
	class TriangleStrip {
			PREVENT_COPY(TriangleStrip);
		public:
			TriangleStrip(const int numberOfTriangles);
			void push_back(const GLfloat& x, const GLfloat& y, const GLfloat& z);
			void push_back3f(const GLfloat* xyz);
			void reset() const {_it = _verts.get();}
			bool at_end() const {return _it >= _ins;}
			void draw() const {glVertex3fv(_it);_it+=3;}
		private:
			std::unique_ptr<GLfloat []> _verts;
			GLfloat * _ins;
			GLfloat * _end;
			mutable GLfloat * _it;
	};

	class SphereCamera : public GameObject {
		public:
			SphereCamera(int drawOrder, double circleY = 5, double circleRadius = 5);
			void update(const UpdateContext& ctx);
			void draw(const DrawContext& ctx);
		private:
			const double _circleY;
			const double _circleRadius;
			double _theta;
			double _delta;
	};


}



