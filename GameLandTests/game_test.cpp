/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#include <test_util.h>
#include <game.h>
#include <game_math.h>

#define TEST template<> template<> void test_group<gamedata>::object::test
namespace tut {
	using namespace game;
	struct gamedata {
	    virtual ~gamedata(){}
    };
	test_group<gamedata> gameTests("030 Game Subsystem");

	Scalar sum_c(const Vector&c) {return c.x() + c.y() + c.z(); }

	void ensure_equals(const char *msg, const Vector& a, const Vector& b) {
		std::ostringstream os;
		os << msg << " " << b;
		for (int i = 0; i < 4; i++)
			ensure_equalsf(os.str().c_str(),a(i),b(i));
	}

	TEST<1>() {
		DrawContext ctx(false,10,10,true);
		ShaderProgram p;
		ensure_error(p.initialise(ctx,"hello","there"),"syntax error");
	}

	TEST<2>() {
		set_test_name("vector operations");
		Vector a(1,2,3);
		ensure_equals("elements are incorrect", a.x() + a.y() + a.z(), 6);
		ensure_equalsf("norm is incorrect", a.norm(), 3.74166);
		Vector b(9,8,7);
		Vector c(a + b);
		ensure_equals("elements are incorrect", c.x() + c.y() + c.z(), 30);
	  c = a - b;
		ensure_equals("elements are incorrect", c.x() + c.y() + c.z(), -18);
		c = a * 2;
		ensure_equals("elements are incorrect", c.x() + c.y() + c.z(), 12);
	}

	TEST<3>() {
		set_test_name("vector calculations");
		const Vector a(10,2,30);
		const Vector b(3,9,4);
		ensure_equalsf("dot incorrect",168, dot(a,b));
		ensure_equalsf("cross incorrect", sum_c(cross_product(a,b)), -sum_c(cross_product(b,a)));
		ensure_equalsf("cross of parallels", sum_c(cross_product(a,a)),0);
	}

	TEST<4>() {
		set_test_name("matrix basics");
		Matrix id;
		ensure_equalsf("id incorrect", 1, *id.c_elems());
		ensure_equalsf("id incorrect", 0, *(id.c_elems()+1));
		ensure_equalsf("id incorrect", 1, *(id.c_elems()+15));
		Matrix b(id * 2);
		ensure_equalsf("multiply incorrect", 2, *(b.c_elems()+15));
		Matrix c(id + b);
		ensure_equalsf("add incorrect", 3, c(3,3));
		ensure_equalsf("add incorrect", 0, c(2,3));
	}

	TEST<5>() {
		set_test_name("matrix multiply");
		const Matrix id;
		const Matrix b(id * 2);
		const Matrix c(id * b);
		ensure_equalsf("matrix multiply incorrect",2,c(1,1));
	}


	TEST<6>() {
		set_test_name("matrix operations");
		Vector u(1,1,0);
		ensure_equalsf("pi is wrong",3.14159,pi);
		Matrix rx(MatrixOp::rotateX,pi);
		ensure_equals("rx",Vector(1,-1,0),(rx * u));
		Matrix ry(MatrixOp::rotateY,pi);
		ensure_equals("ry",Vector(-1,1,0), (ry * u));
		Matrix rz(MatrixOp::rotateZ,pi);
		ensure_equals("rz",Vector(-1,-1,0), (rz * u));
		Matrix rs(MatrixOp::scale,Vector(2,2,2));
		ensure_equals("rs",Vector(2,2,0), (rs * u));
		Matrix rt(MatrixOp::translate,Vector(1,1,1));
		ensure_equals("rt",Vector(2,2,1),(rt * u));
	}

	TEST<7>() {
		set_test_name("matrix combination operations");
		Vector u(1,1,0);
		Matrix rx(MatrixOp::rotateX,pi);
		Matrix ry(MatrixOp::rotateY,pi);
		Matrix rz(MatrixOp::rotateZ,pi);
		Matrix rs(MatrixOp::scale,Vector(2,2,2));
		Matrix rt(MatrixOp::translate,Vector(1,1,1));
		ensure_equals("rx*ry",Vector(-1,-1,0), (rx*ry)*u);
		ensure_equals("rx*ry*rs",Vector(-2,-2,0), (rx*ry*rs)*u);
		ensure_equals("rt*rx*ry",Vector(0,0,1), (rt*rx*ry)*u);
		ensure_equals("rs*rt*rt*rx*ry",Vector(2,2,4), (rs*rt*rt*rx*ry)*u);
	}

	TEST<8>() {
		set_test_name("load bmp");
		ResourceContext ctx;
		game::Surface s("media/cracked_tiles.BMP");
	}

	TEST<9>() {
		set_test_name("load BMP fails");
		ResourceContext ctx;
		ensure_error(game::Surface("_not_a_name.bmp"),"name.bmp");
	}

	TEST<10>() {
		set_test_name("load opengl surface");
		ResourceContext ctx;
		DrawContext dx(false,10,10,true);
		game::Surface surface("media/cracked_tiles.BMP");
		Texture t(dx);
		t.bind(1);
		t.copy_from(surface.sdl());
	}

	TEST<11>() {
		set_test_name("translation of a point");
		Vector pointToTranslate(Vector::north);
		Matrix xlator(MatrixOp::translate, Vector::north * 2.0f);
		auto newPoint = xlator * pointToTranslate;
		ensure_equals(pointToTranslate * 3.0f, newPoint);
	}

	TEST<12>() {
		Vector2 a(1,0),b(0,0);
		Vector2 c = a.towards(b,0.9f);
		Vector2 d = b.towards(a,0.9f);
		ensure_equals(c,Vector2(0.1,0));
		ensure_equals(d,Vector2(0.9,0));
	}
}
