#include "main_scene.h"
#include "../systemex/log.h"
#include <iostream>
using namespace duality;
using namespace game;

int main( int , char* [] ) {
	try {
		game::Game g(
				MainObject::u_ptr(new DualityScene()),
				new game::UpdateContext(2),
				new game::DrawContext(false,640,480),
				new game::ResourceContext("duality/")
		);
		g.run();
	} catch (std::exception &e) {
		LOG << "exception: " << (e.what()) << std::endl;
	} catch (...) {
		LOG << "unexpected error occurred";
	}
	return EXIT_SUCCESS;
}
