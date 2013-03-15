/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#pragma once

#include "../game/game.h"
#include "../game/collisions2d.h"

namespace duality {
	using game::SimpleCollisionManager;

	class DualityScene: public game::MainObject {
	public:
		DualityScene();
	private:
		SimpleCollisionManager _col_mgr;
};

}

