/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#include "main_scene.h"


using namespace game;
const GLdouble PlayScene::NEAREST = ARENA_HALF / 10.0;
const GLdouble PlayScene::FAREST = CAM_RADIUS * 3;
const GLfloat FENCE_Z =  FLOOR_Z + ARENA_HALF / 10.0f;
auto FENCE_TYPE = object_t::Fence;

const GLfloat leftBack[] =   { -ARENA_HALF,  -ARENA_HALF, FLOOR_Z };
const GLfloat rightBack[] =  { +ARENA_HALF,  -ARENA_HALF, FLOOR_Z };
const GLfloat rightFront[] = { +ARENA_HALF,  +ARENA_HALF, FLOOR_Z };
const GLfloat leftFront[] =  { -ARENA_HALF,  +ARENA_HALF, FLOOR_Z };

const GLfloat leftBackT[] =   { -ARENA_HALF,  -ARENA_HALF, FENCE_Z };
const GLfloat rightBackT[] =  { +ARENA_HALF,  -ARENA_HALF, FENCE_Z };
const GLfloat rightFrontT[] = { +ARENA_HALF,  +ARENA_HALF, FENCE_Z };
const GLfloat leftFrontT[] =  { -ARENA_HALF,  +ARENA_HALF, FENCE_Z };

Vector2 toVec(const GLfloat v[]) {return Vector2(v[0],v[1]);}
Vector2 toVec(const GLfloat v[], const Vector2 &displace) {return Vector2(v[0] + displace.x(),v[1] + displace.y());}

CollidablePolygon toPoly(const GLfloat v[], const GLfloat w[], const Vector2 &d) {
	return CollidablePolygon(FENCE_TYPE, toVec(v), {toVec(v,d), toVec(w,d), toVec(w)});
}

Fence::Fence() : _strip(8),
	    _program_p(), _tex_p(), _polys(
	    		{toPoly(leftBack,rightBack,Vector2(0,-1)),
	    		 toPoly(leftFront,rightFront,Vector2(0,1)),
	    		 toPoly(leftBack,leftFront,Vector(-1,0)),
	    		 toPoly(rightBack,rightFront,Vector(1,0))})
	 {}

void Fence::initialise(const ResourceContext &ctx, const DrawContext &draw) {
		_strip.push_back({
			leftBack, leftBackT,
			rightBack, rightBackT,
			rightFront, rightFrontT,
			leftFront,leftFrontT,
			leftBack, leftBackT
		});
		_program_p.reset(ctx.load_program(draw,"fence"));
		_tex_p.reset(ctx.load_texture_bmp(draw,"../cracked_tiles.bmp",0));

	}

void Fence::draw(const DrawContext &ctx) {
	    _tex_p->activate();
        _program_p->begin();
        _program_p->arg("tex",_tex_p->index());
		glBegin(GL_TRIANGLE_STRIP);
		_strip.draw();
		glEnd();
		_program_p->end();
	}

CollidablePolygon * Fence::poly(int i) {
		return &_polys[i];
}

class Arena : public game::GameObject {
public:
	Arena() : _strip(2),_program_p(),_tex_p() {};

	void initialise(const ResourceContext &ctx, const DrawContext &draw) override {
		_strip.push_back({leftFront,leftBack,rightFront,rightBack});
		_program_p.reset(ctx.load_program(draw,"arena"));
		_tex_p.reset(ctx.load_texture_bmp(draw,"../cracked_tiles.bmp",0));
	}

	void draw(const DrawContext& draw) override {
	    _tex_p->activate();
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
	void on_collide(CollidablePolygon &a, CollidablePolygon &b) {}
};

PlayScene::PlayScene() :
		_col_mgr(new DualityListener(),
		         BoundedBox2(toVec(leftBack), toVec(rightFront)),5),
		_snake(new SnakeObject(_col_mgr)),
		_fence(new Fence())

			{
	add_part(new SphereCamera(-1, CAM_Y, CAM_RADIUS));
	add_part(GameObject::u_ptr(new Arena()));
	add_part(_snake);
	add_part(_fence);
	set_active(false);
	set_hidden(false);
}

void PlayScene::update(const UpdateContext &uc) {
	if (uc.input().key_up() == SDLK_ESCAPE)
		activate_next();
	else
		GameObjectChainLink::update(uc);
};

void PlayScene::activate() {
	 _snake->reset();GameObjectChainLink::activate();
	for (int i=0; i < 4; i++)
		_snake->mgr().add(_fence->poly(i));
	GameObjectChainLink::activate();
}
