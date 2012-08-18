#include "main_scene.h"
#include <iostream>
using namespace duality;

int main( int , char* [] ) {
	try {
		game::Game g(
				new DualityScene(),
				new game::UpdateContext(2),
				new game::DrawContext(false,640,480)
		);
		g.run();
	} catch (std::exception &e) {
		std::cout << "exception: " << (e.what());
	} catch (...) {
		std::cout << "unexpected error occurred";
	}
	return EXIT_SUCCESS;
}
