/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#pragma once

#include "../game/game.h"
#include "../game/collisions2d.h"
#include "../game/game_objects.h"
#include "snake.h"
#define ARENA_HALF 3.0f
#define FLOOR_Z 0.0f
#define CAM_Y ARENA_HALF * 1.5
#define CAM_RADIUS ARENA_HALF * 1.2

namespace duality {
	using namespace game;

	class Fence : public game::GameObject {
	public:
		Fence();
		void initialise(const ResourceContext &ctx, const DrawContext &draw) override;
		void draw(const DrawContext &ctx) override;
		CollidablePolygon * poly(int i);
	private:
		TriangleStrip _strip;
		ShaderProgram::u_ptr _program_p;
		Texture::u_ptr _tex_p;
		CollidablePolygon _polys[4];
	};

	/**
	 * The scene where the game is played
	 */
	class DualityScene : public game::GameObjectChainLink {
		PREVENT_COPY(DualityScene)
	public:
		DualityScene();
		void update(const UpdateContext &uc) override;
		void activate() override;
	private:
		CollisionManagerWithBoxes _col_mgr;
	public:
		SnakeObject * _snake;
		Fence * _fence;
		static const GLdouble NEAREST;
		static const GLdouble FAREST;
	};

}

