#include "../systemex/log.h"
#include <iostream>
#include "../game/game.h"
#include "Heightmap.h"

using namespace game;
using namespace terrain;

class TerrainController : public MainObject {

};

int main( int , char* [] ) {
	HeightmapWithByte<50,50> hm;
	hm.read_from_bmp("terrain/terrain.bmp");
//	game::Game g(new TerrainController(), "terrain/");
//	return g.run();
}
