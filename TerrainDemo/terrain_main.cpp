
#include <math.h>
#include <log.h>
#include <iostream>
#include <game.h>
#include <terrain.h>
#include "terrain_object.h"

using namespace game;
using namespace terrain;

const size_t map_size = 128;

typedef HeightmapWithByte<map_size,map_size> demo_hm_t;

typedef TerrainObject<Byte,demo_hm_t,TransformerByte> base_terrain_t;

const float square_size = 0.5f;

const Vector terrain_center(square_size*map_size/2,-square_size*map_size/2,0.0f);

class DemoTerrain : public base_terrain_t {
public:
	DemoTerrain() : TerrainObject(
			new demo_hm_t(),
			TransformerByte(square_size,0.0f,10.0f))  {}

	void initialise(const ResourceContext & rctx, const DrawContext& dctx) override {
		_hmap->read_from_bmp(rctx.dir() + "test_terrain.bmp");
		_hmap->normalise();
		base_terrain_t::initialise(rctx,dctx);
	}

	void draw(const game::DrawContext& dc) override {
		base_terrain_t::draw(dc);
    glLineWidth(10.0f); //size in pixels
    auto sw = floor_south_west();
    auto ne = floor_north_east();
    const int steps = 2000;
    auto inc = (sw - ne) / (steps * 1.0f);
    glBegin(GL_LINES); 
      for (int i=0;i<steps;i++) {
      	auto vi = ne + (inc*i);
      	auto fi = floor_at(vi.x(),vi.y());
      	auto ni = normal_at(vi.x(),vi.y());
      	ni.normalise();
      	auto ei = fi + ni;
    		glColor3f(0,0,1);
      	glVertex3f(fi.x(),fi.y(),fi.z());  
    		glColor3f(1,0,0);
      	glVertex3f(ei.x(),ei.y(),ei.z()+1);    	
      }
    glEnd();
    glColor3f(1,1,1);
   // draw_wire();
	}
};

class TerrainController : public MainObject {
	public:
	TerrainController() : MainObject(-100,square_size/10.0f,square_size * map_size) {
		add_part(new SphereCamera(-1,5.0f,3.0f, terrain_center));
		add_part(new DemoTerrain());
	}
};

int main( int , char* [] ) {
	LOG << "started terrain demo";
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
