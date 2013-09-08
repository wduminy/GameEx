#include <test_util.h>
#define TESTDATA TerrainData
#define TEST(N) template<> template<> void test_group<tut::TESTDATA>::object::test<N>()
#define BEGIN(Num,Name) template<> template<> void test_group<TESTDATA>::object::test<Num>() {set_test_name(Name);
#define END }
#include <terrain.h>

namespace tut {
	using namespace terrain;
	struct TESTDATA{
		virtual ~TESTDATA(){}
	};
	test_group<TESTDATA> teterrrainTests("110 Terrain tests");
	BEGIN(1, "reading and writing BMP Heightmap") {
		HeightmapWithByte<50,50> hm;
		hm.read_from_bmp("media/terrain/test.bmp");
		for_each(e,hm.elems()) {
			*e = 255 - *e;
		}
		hm.write_to_bmp("my_hm.bmp");
	} END
}


