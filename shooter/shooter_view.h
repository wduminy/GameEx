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
	SDL_Rect _draw_dst;
	int _map_left;
	int _row_at_top;
	std::array<int,LEVEL_SIZE> _tile_x, _tile_y;
};