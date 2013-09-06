/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#include "controller.h"
#include <log.h>
#include <iostream>
using namespace game;

int main( int , char* [] ) {
	LOG << "started";
	try {
		auto dc = new DrawContext(false,800,600,true);
		auto uc = new UpdateContext(50,20);
		game::Game g(
				MainObject::u_ptr(new Controller()),
				UpdateContext::u_ptr(uc),
				DrawContext::u_ptr(dc),
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
