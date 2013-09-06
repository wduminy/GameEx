/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#pragma once
#include <game.h>
#include "front_page.h"
using namespace game;

class Controller: public MainObject {
	PREVENT_COPY(Controller)
public:
	Controller();
	void update(const UpdateContext& ctx) override;
private:
	/** The object in the link chain that causes this
	 *  controller to stop when it is not active.
	 */
	FrontPage * _main_page;
};
