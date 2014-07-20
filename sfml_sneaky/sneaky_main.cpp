// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include "game.h"
#include "sneakyworld.h"
#include <iostream>
int main()
{
	try {
		sneaky::SneakyGame().run();
	} catch (std::runtime_error &e) {
		std::cout << "Error:" << e.what();
	}
	return 0;
}
