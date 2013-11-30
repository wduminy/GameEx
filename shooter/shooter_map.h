#pragma once
#include <game_base.h>
#include <array>
#include "shooter_state.h"


class ShooterMapView : public game::GameObject  {
public:
	ShooterMapView(const LevelState * state);
	void initialise(const game::ResourceContext &rc, const game::DrawContext &dc) override;
	void draw(const game::DrawContext &dc) override;
	void update(const game::UpdateContext &uc) override;
private:
	void update_tile_indexes();
	void calculate_tile(const int x, const int y, int &t_x, int &t_y); 
	void draw_ground(const game::DrawContext &dc);
	SDL_Rect _draw_dst;
	int _map_left;
	int _row_at_top;
	const LevelState * _state;
	std::array<int,LEVEL_SIZE> _tile_x, _tile_y;
};