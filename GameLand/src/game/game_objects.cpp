/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#include "game_objects.h"
#include <type_traits>
namespace game {
	const static double a360 = 3.14 * 2.0;
	const static double degrees_per_second = 90;
	const static double rads_per_second = a360 * degrees_per_second / 360.0;

	TriangleStrip::TriangleStrip(const int numberOfTriangles) :
		_verts(new GLfloat[(numberOfTriangles+2)*3]),
		_ins(_verts.get()),
		_end(_verts.get() + (numberOfTriangles+2)*3),
		_it(_ins)
	{}
	void TriangleStrip::push_back(const GLfloat& x, const GLfloat& y, const GLfloat& z) {
		if (_ins >= _end)
			throw std::runtime_error("strip past is capacity");
		*(_ins++) = x;
		*(_ins++) = y;
		*(_ins++) = z;
	}

	void TriangleStrip::push_back3f(const GLfloat* xyz) {
		push_back(xyz[0],xyz[1],xyz[2]);
	}

	void TriangleStrip::push_back(const vector<const GLfloat*> &points) {
		for_each(e,points) {
				push_back3f(*e);
		}
	}

	void TriangleStrip::push_back_reverse(const vector<const GLfloat*> &points) {
		for (auto e = points.rbegin(); e != points.rend(); e++) {
				push_back3f(*e);
		}
	}

	void TriangleStrip::draw() const {
		reset();
		while (!at_end())
			draw_step();
	}

	SphereCamera::SphereCamera(int drawOrder, double z, double r)
	: GameObject(drawOrder), _circleZ(z), _circleRadius(r), _theta(0), _delta(0) {}

	void SphereCamera::update(const UpdateContext& ctx) {
		const static double rads_per_update = ctx.seconds_per_update() * rads_per_second;
		switch (ctx.input().key_down()) {
			case SDLK_LEFT:
				_delta = -rads_per_update;
				break;
			case SDLK_RIGHT:
				_delta = +rads_per_update;
				break;
			case SDLK_UP:
				_circleZ += rads_per_update;
				break;
			case SDLK_DOWN:
				_circleZ -= rads_per_update;
				break;
			default:
				break;
		}
		switch (ctx.input().key_up()) {
			case SDLK_LEFT:
			case SDLK_RIGHT:
				_delta = 0;
				break;
			default:
				break;
		}
		_theta += _delta;
	}

	void SphereCamera::draw(const DrawContext&) {
		gluLookAt(_circleRadius * sin(_theta),
				  -_circleRadius * cos(_theta),
				  _circleZ,
				       0,0,0,0,0,1);
	}

	/**
	 *
	 * @param x  -1 = left 1 = right
	 * @param y  -1 = bottom 1 = top
	 * @param width 1.0 is full screen
	 * @param height 1.0 is full screen
	 * @param pixelW
	 * @param pixelH
	 */
	Panel::Panel(const Scalar x, const Scalar y, const Scalar width,
			const Scalar height, int pixelW, int pixelH) :
					_rectangle(2),_program(),_tex(),_surface(),
					_left(x), _top(x), _pixelW(pixelW), _pixelH(pixelH), _width(width), _height(height) {
		auto r = x + width;
		auto b = y - height;
		_rectangle.push_back(x,y,0);
		_rectangle.push_back(x,b,0);
		_rectangle.push_back(r,y,0);
		_rectangle.push_back(r,b,0);
	}


	void Panel::initialise(const ResourceContext &rc, const DrawContext& dc) {
		// each game should have its own panel shader
		_program  = rc.load_program(dc.gl(),"panel");
		// TODO At the moment we need to load the bmp to initialise the texture
		// properly, but we should be able to use the texture without doing this
		_tex.reset(new Texture(dc));
		//_surface.reset(new Surface(rc.dir() + "../test.bmp"));
		_surface.reset(new Surface(_pixelW,_pixelH));
		_tex->bind(1);
	}

	void Panel::draw(const DrawContext &dc) {
	    _tex->activate();
        _program->begin();
        if (_program->is_first_time()) {
			_program->arg("tex",_tex->index());
			_program->arg("left", _left);
			_program->arg("top",_top);
			_program->arg("width", _width);
			_program->arg("height",_height);
        }
        glBegin(GL_TRIANGLE_STRIP);
		_rectangle.draw();
		glEnd();
		_program->end();
	}
}

