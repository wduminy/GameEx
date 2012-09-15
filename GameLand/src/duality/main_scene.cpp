/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#include "../game/game_objects.h"
#include "main_scene.h"
namespace duality {
using namespace game;

// a temp function to show initial graphics
void drawTriangles(const TriangleStrip& strip) {
	static GLubyte white[4] = { 255, 255, 255, 255 };
	static GLubyte green[4] = { 0, 255, 0, 255 };
	static GLubyte blue[4] = { 0, 0, 255, 255 };
	static GLubyte red[4] = { 255, 0, 0, 255 };
	static GLubyte * colours[4] = { red, white, green, blue };
	int col = 0;
	strip.reset();
	while (!strip.at_end()) {
		glColor4ubv(colours[col]);
		strip.draw();
		if (col == 3)
			col = 0;
		else
			col++;
	}
}

class Arena : public game::GameObject {
public:
	Arena() : _strip(2),_program_p() {};

	void initialise(const ResourceContext &ctx, const DrawContext &draw) override {
		const float arenaHalf = 2.0f;
		const float floorLevl = 0.0f;
		static GLfloat leftBack[] = { -arenaHalf, floorLevl, -arenaHalf };
		static GLfloat rightBack[] = { +arenaHalf, floorLevl, -arenaHalf };
		static GLfloat rightFront[] = { +arenaHalf, floorLevl, +arenaHalf };
		static GLfloat leftFront[] = { -arenaHalf, floorLevl, +arenaHalf };
		_strip.push_back3f(leftFront);
		_strip.push_back3f(rightFront);
		_strip.push_back3f(leftBack);
		_strip.push_back3f(rightBack);
		_program_p = ShaderProgram::u_ptr(new ShaderProgram(draw.gl()));
		_program_p->bind(ctx.load_text("arena.vert"), ctx.load_text("arena.frag"));
	}

	void draw(const DrawContext& draw) override {
		glBegin(GL_TRIANGLE_STRIP);
// TODO (willemd#1#): use test shader
		drawTriangles(_strip);
		glEnd();
	}

private:
	TriangleStrip _strip;
	ShaderProgram::u_ptr _program_p;
};


DualityScene::DualityScene() : MainObject(-100) {
	add_part(GameObject::u_ptr(new Arena()));
	add_part(GameObject::u_ptr(new SphereCamera(drawOrder() + 1)));
}


}
