// see licence.txt
#include "game.h"
#include <array>
#pragma once
namespace game {

	class VertexBuffer {
	};

  // triangles are specified anti-clockwise
	class TriangleStrip {
			PREVENT_COPY(TriangleStrip);
		public:
			TriangleStrip(const int numberOfTriangles);
			void push_back(const GLfloat& x, const GLfloat& y, const GLfloat& z);
			void push_back3f(const GLfloat* xyz);
			void reset() const {_it = _verts;}
			bool at_end() const {return _it >= _ins;}
			void draw() const {glVertex3fv(_it);_it+=3;}
			~TriangleStrip();
		private:
			GLfloat * _verts;
			mutable GLfloat * _it;
			GLfloat * _ins;
			GLfloat * _end;
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



