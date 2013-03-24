/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#include "game.h"
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

	class BillBoard {
	public:
		BillBoard(const Scalar left, Scalar top, const Scalar width, const Scalar height);
		BillBoard(const Vector &p1, const Vector &p2);
		void draw() {_rectangle.draw();}
	private:
		TriangleStrip _rectangle;
	};

	class SphereCamera : public GameObject {
		public:
			SphereCamera(int drawOrder, double circleZ = 5, double circleRadius = 5);
			void update(const UpdateContext& ctx);
			void draw(const DrawContext& ctx);
		private:
			double _circleZ;
			const double _circleRadius;
			double _theta;
			double _delta;
	};


}



