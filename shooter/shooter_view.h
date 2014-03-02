#error view must be deleted
#pragma once
#include <game_base.h>
#include <array>
#include "shooter_state.h"

/** Draws the game state */
class ShooterView : public ShooterStateObject  {
public:
	ShooterView();
	void initialise(const game::ResourceContext &rc, const game::DrawContext &dc) override;
	void draw(const game::DrawContext &dc) override;
private:
	void update_tile_indexes();
	void calculate_tile(const int x, const int y, int &t_x, int &t_y); 
	SDL_Rect draw_dst_;
	int map_left_;
	int row_at_top_;
	std::array<int,LEVEL_SIZE> tile_x_, tile_y_;
};
