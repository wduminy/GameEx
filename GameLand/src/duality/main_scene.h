/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#pragma once

#include "../game/game.h"
#include "../game/collisions2d.h"

namespace duality {
	using game::CollisionManagerWithBoxes;

	/**
	 * The scene where the game is played
	 */
	class DualityScene : public game::GameObjectChainLink {
	public:
		DualityScene();
	private:
		CollisionManagerWithBoxes _col_mgr;
	public:
		static const GLdouble NEAREST;
		static const GLdouble FAREST;
	};

}

