#include <systemex/log.h>
#include <iostream>
#include <game/game.h>
#include <terrain/terrain.h>

using namespace game;
using namespace terrain;

typedef HeightmapWithByte<50,50> demo_hm_t;

class DemoTerrain : public TerrainObject<Byte,demo_hm_t,TransformerByte> {
public:
	DemoTerrain() : TerrainObject(
			new demo_hm_t(),
			TransformerByte(1.0f,0.0f,5.0f))  {}
	void initialise(const ResourceContext & rctx, const DrawContext& dctx) override {
		_hmap->read_from_bmp(rctx.dir() + "test.bmp");
	}
};

class TerrainController : public MainObject {
	public:
	TerrainController() : MainObject(-100,0.3,100) {
		add_part(new SphereCamera(-1,10.0f,50.0f, Vector(25.0f,0.0f,0.0f)));
		add_part(new DemoTerrain());
	}
};

int main( int , char* [] ) {
	game::Game g(new TerrainController(), "terrain/", 50, 5000);
	return g.run();
}
