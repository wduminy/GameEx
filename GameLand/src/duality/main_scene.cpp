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
const float FLOOR_Y = 0.0f;
static const double CAM_Y = ARENA_HALF * 1.5;
static const double CAM_RADIUS = ARENA_HALF * 1.8;
static const GLdouble NEAREST = ARENA_HALF / 10.0;
static const GLdouble FAREST = CAM_RADIUS * 2;

class Arena : public game::GameObject {
public:
	Arena() : _strip(2),_program_p(),_tex_p() {};

	void initialise(const ResourceContext &ctx, const DrawContext &draw) override {
		static GLfloat leftBack[] = { -ARENA_HALF, FLOOR_Y, -ARENA_HALF };
		static GLfloat rightBack[] = { +ARENA_HALF, FLOOR_Y, -ARENA_HALF };
		static GLfloat rightFront[] = { +ARENA_HALF, FLOOR_Y, +ARENA_HALF };
		static GLfloat leftFront[] = { -ARENA_HALF, FLOOR_Y, +ARENA_HALF };
		_strip.push_back3f(leftFront);
		_strip.push_back3f(rightFront);
		_strip.push_back3f(leftBack);
		_strip.push_back3f(rightBack);
		_program_p = ShaderProgram::u_ptr(new ShaderProgram(draw.gl()));
		_program_p->bind(ctx.load_text("arena.vert"), ctx.load_text("arena.frag"));
		_tex_p = Texture::u_ptr(new Texture(draw.gl()));
		_tex_p->copy_from(*ctx.load_BMP("../cracked_tiles.bmp"));
        _tex_p->activate(GL_TEXTURE1);
	}

	void draw(const DrawContext& draw) override {
        _program_p->begin();
        _program_p->arg("tex",1);
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


DualityScene::DualityScene() : MainObject(-100,NEAREST,FAREST) {
	add_part(GameObject::u_ptr(new Arena()));
	add_part(GameObject::u_ptr(new SphereCamera(draw_order() + 1, CAM_Y, CAM_RADIUS)));
	add_part(GameObject::u_ptr(new SnakeObject()));
}


}
