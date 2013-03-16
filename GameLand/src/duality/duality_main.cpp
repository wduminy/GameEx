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
				//UpdateContext::u_ptr(new UpdateContext(10000,10)), // to check performance
				UpdateContext::u_ptr(new UpdateContext(50,20)),
				DrawContext::u_ptr(new DrawContext(false,800,600)),
				ResourceContext::u_ptr(new ResourceContext("duality/"))
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
