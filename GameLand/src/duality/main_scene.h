/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#pragma once

#include "../game/game.h"
#include "../game/collisions2d.h"
#define ARENA_HALF 3.0f
#define FLOOR_Z 0.0f
#define CAM_Y ARENA_HALF * 1.5
#define CAM_RADIUS ARENA_HALF * 1.5

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

