#pragma once
#include <game_base.h>

class ShooterMap : public game::GameObject  {
public:
	ShooterMap();
	void initialise (const game::ResourceContext &rc, const game::DrawContext &dc) override;
	void draw(const game::DrawContext &dc) override;
	void update(const game::UpdateContext &uc) override;
private:
	SDL_Rect _draw_dst;
	int _map_top;
	int _map_left;
};