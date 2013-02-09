/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#include "../game/game_objects.h"
#include "main_scene.h"
#include "snake.h"

namespace duality {
using namespace game;
const float ARENA_HALF = 3.0f;
const float FLOOR_Z = 0.0f;
static const double CAM_Y = ARENA_HALF * 1.5;
static const double CAM_RADIUS = ARENA_HALF * 1.5;
static const GLdouble NEAREST = ARENA_HALF / 10.0;
static const GLdouble FAREST = CAM_RADIUS * 2;

class Arena : public game::GameObject {
public:
	Arena() : _strip(2),_program_p(),_tex_p() {};

	void initialise(const ResourceContext &ctx, const DrawContext &draw) override {
		static GLfloat leftBack[] =   { -ARENA_HALF,  -ARENA_HALF, FLOOR_Z };
		static GLfloat rightBack[] =  { +ARENA_HALF,  -ARENA_HALF, FLOOR_Z };
		static GLfloat rightFront[] = { +ARENA_HALF,  +ARENA_HALF, FLOOR_Z };
		static GLfloat leftFront[] =  { -ARENA_HALF,  +ARENA_HALF, FLOOR_Z };
		_strip.push_back3f(leftFront);
		_strip.push_back3f(leftBack);
		_strip.push_back3f(rightFront);
		_strip.push_back3f(rightBack);
		_program_p = ctx.load_program(draw.gl(),"arena");
		_tex_p = ctx.load_texture_bmp(draw.gl(),"../cracked_tiles.bmp",0);
	}

	void draw(const DrawContext& draw) override {
	    _tex_p->activate(_tex_p->index());
        _program_p->begin();
        _program_p->arg("tex",_tex_p->index());
		glBegin(GL_TRIANGLE_STRIP);
		_strip.draw();
		glEnd();
		_program_p->end();
	}

private:
	TriangleStrip _strip;
	ShaderProgram::u_ptr _program_p;
	Texture::u_ptr _tex_p;
};

class DualityListener : public CollisionListener {
public:
	void on_collide(CollidablePolygon &a, CollidablePolygon &b) {

	}
};

DualityScene::DualityScene() : MainObject(-100,NEAREST,FAREST),
		_col_mgr(CollisionListener::u_ptr(new DualityListener())) {
	add_part(GameObject::u_ptr(new Arena()));
	add_part(GameObject::u_ptr(new SphereCamera(draw_order() + 1, CAM_Y, CAM_RADIUS)));
	add_part(GameObject::u_ptr(new SnakeObject(_col_mgr)));
}


}
