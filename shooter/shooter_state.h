#pragma once
#include <game_base.h>
#include <game_math.h>
#include <collisions2d.h>
#include "shooter.h"
#include "warzone.h"
#include "drome.h"
#include "shooter_constants.h"

/** The shooter game state */
class ShooterState : public game::GameObjectWithDynamicParts {
private:
	game::CollisionManagerWithBoxes col_mgr_;
	std::unique_ptr<Shooter> shooter_;
	WarZone * map_;
	bool left_key_down_, right_key_down_, up_key_down_, down_key_down_;
	SDL_Rect draw_dst_;
	int row_at_top_;
	std::array<int,LEVEL_SIZE> tile_x_, tile_y_;
public:
	ShooterState();
	void initialise(const game::ResourceContext &rctx, const game::DrawContext &dc) override;
	void update(const game::GameContext & uc) override;
	void draw(const game::DrawContext &dc) override;
private:
	const Shooter& shooter() const {ASSERT(shooter_); return *shooter_; }
	void load_dromes(const std::string &file_name);
};
