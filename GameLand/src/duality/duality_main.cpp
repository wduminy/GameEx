/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#include "main_scene.h"
#include "../systemex/log.h"
#include <iostream>
using namespace duality;
using namespace game;

int main( int , char* [] ) {
	try {
		game::Game g(
				MainObject::u_ptr(new DualityScene()),
				UpdateContext::u_ptr(new UpdateContext(48,24)),
				new game::DrawContext(false,800,600),
				new game::ResourceContext("duality/")
		);
		g.run();
	} catch (std::exception &e) {
		LOG << "exception: " << (e.what()) << std::endl;
        return EXIT_FAILURE;
  	} catch (...) {
		LOG << "unexpected error occurred";
        return EXIT_FAILURE;
    }
	return EXIT_SUCCESS;
}
