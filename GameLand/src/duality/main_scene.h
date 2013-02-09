/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#pragma once

#include "../game/game.h"
#include "../game/collisions2d.h"

namespace duality {
	using game::CollisionManager;

	class DualityScene: public game::MainObject {
	public:
		DualityScene();
	private:
		CollisionManager _col_mgr;
};

}

