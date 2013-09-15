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
		ensure_equals(hm.max_height(),255);
	} END

	BEGIN(3, "finding triangles") {
		Byte h[] = {0,0,0};
		int at = 0;
		auto fun = [&] (int c, int r, Byte v) { h[at++] = v; };
		hm.apply_triangle(2.4,2.1,fun);
		ensure_equals(at,3);
		ensure_equals((int)h[1],92);
	} END
}


