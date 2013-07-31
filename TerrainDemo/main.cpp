#include <systemex/log.h>
#include <iostream>
#include <game/game.h>
#include <terrain/terrain.h>
#include "terrain_object.h"
using namespace game;
using namespace terrain;

typedef HeightmapWithByte<128,128> demo_hm_t;

class DemoTerrain : public TerrainObject<Byte,demo_hm_t,TransformerByte> {
public:
	DemoTerrain() : TerrainObject(
			new demo_hm_t(),
			TransformerByte(0.5f,-10.0f,10.0f))  {}
	void initialise(const ResourceContext & rctx, const DrawContext& dctx) override {
		_hmap->read_from_bmp(rctx.dir() + "riemer_terrain.bmp");
		TerrainObject<Byte,demo_hm_t,TransformerByte>::initialise(rctx,dctx);
	}
};

class TerrainController : public MainObject {
	public:
	TerrainController() : MainObject(-100,0.3,100) {
		add_part(new SphereCamera(-1,20.0f,50.0f, Vector(25.0f,0.0f,0.0f)));
		add_part(new DemoTerrain());
	}
};

int main( int , char* [] ) {
	LOG << "started terrain demo ";
	try {
		game::Game g(new TerrainController(), "terrain/", 50, 5000);
		return g.run();
	} catch (std::exception &e) {
		LOG << "exception: " << (e.what()) << std::endl;
        return EXIT_FAILURE;
  	} catch (...) {
		LOG << "unexpected error occurred";
        return EXIT_FAILURE;
    }
	return EXIT_SUCCESS;

}
