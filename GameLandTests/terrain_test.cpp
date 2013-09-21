#include <test_util.h>
#define TESTDATA TerrainData
#define TEST(N) template<> template<> void test_group<tut::TESTDATA>::object::test<N>()
#define BEGIN(Num,Name) template<> template<> void test_group<TESTDATA>::object::test<Num>() {set_test_name(Name);
#define END }
#include <terrain.h>

namespace tut {
	using namespace terrain;
	using namespace game;
	struct TESTDATA{
		HeightmapWithByte<50,50> hm;
		TESTDATA() {
			hm.read_from_bmp("media/terrain/test.bmp");			
		}
		void ensure_cols(Scalar x, Scalar y, int c1, int c2, int c3) {
			int col[] = {0,0,0};
			int at = 0;
			auto fun = [&] (size_t c, size_t r, Byte v) { col[at++] = c; };
			hm.apply_triangle(x,y,fun);
			std::string str = systemex::string_from_format("for coordinates (%f,%f) ",x,y);
			ensure_equals(at,3);
			ensure_equals((str + "point 1"),(int)col[0],c1);
			ensure_equals((str + "point 2"),(int)col[1],c2);
			ensure_equals((str + "point 3"),(int)col[2],c3);
		}
		void ensure_rows(Scalar x, Scalar y, int r1, int r2, int r3) {
			int row[] = {0,0,0};
			int at = 0;
			auto fun = [&] (int c, int r, Byte v) { row[at++] = r; };
			hm.apply_triangle(x,y,fun);
			std::string str = systemex::string_from_format("for coordinates (%f,%f) ",x,y);
			ensure_equals(at,3);
			ensure_equals((str + "row point 1"),(int)row[0],r1);
			ensure_equals((str + "row point 2"),(int)row[1],r2);
			ensure_equals((str + "row point 3"),(int)row[2],r3);
		}
		virtual ~TESTDATA(){}
	};
	test_group<TESTDATA> terrainTests("110 Terrain tests");
	BEGIN(1, "reading and writing BMP Heightmap") {
		for_each(e,hm.elems()) {
			*e = 255 - *e;
		}
		hm.write_to_bmp("my_hm.bmp");
	} END

	BEGIN(2, "normalising a heightmap") {
		hm.normalise();
		ensure_equals((int)hm.max_height(),255);
	} END

	BEGIN(3, "finding triangles") {
		ensure_cols(0,0,1,0,1);
		ensure_cols(0.2f,0.1f,0,1,0);
		ensure_cols(0.1f,0.2f,1,0,1);
		ensure_rows(0,0,0,0,1);
		ensure_rows(0.2f,0.1f,0,1,1);
		ensure_rows(0.1f,0.2f,0,0,1);
	} END

	BEGIN(4, "finding triangles min") {
		Byte h[] = {0,0,0};
		int at = 0;
		auto fun = [&] (int c, int r, Byte v) { h[at++] = v; };
		hm.apply_triangle(0,0,fun);
		ensure_equals(at,3);
		ensure_equals((int)h[1],73);
	} END

	BEGIN(5, "finding triangles max") {
		Byte h[] = {0,0,0};
		int at = 0;
		auto fun = [&] (int c, int r, Byte v) { h[at++] = v; };
		hm.apply_triangle(49,49,fun);
		ensure_equals(at,3);
		ensure_equals((int)h[1],41);
	} END
}


