#pragma once
#include <game_base.h>
#include <array>
#include "shooter_state.h"

#define LEVEL_SIZE MAP_WIDTH*LEVEL_HEIGHT

enum class GroundCover {Road,Sand,Water,Grass,Scrub,Bush};  

/** The ground map is eye candy -- gameplay is not influenced by it */
class GroundMap {
public:
	GroundMap(const std::string &file_name);
	void calculate_tile(const int x, const int y, int &t_x, int &t_y); 
private:
	std::array<GroundCover,LEVEL_SIZE> _data;
public:
	typedef std::unique_ptr<GroundMap> u_ptr;	
};

class ShooterMapView : public game::GameObject  {
public:
	ShooterMapView(const LevelState * state);
	void initialise(const game::ResourceContext &rc, const game::DrawContext &dc) override;
	void draw(const game::DrawContext &dc) override;
	void update(const game::UpdateContext &uc) override;
private:
	void draw_ground(const game::DrawContext &dc);
	SDL_Rect _draw_dst;
	int _map_left;
	GroundMap::u_ptr _ground_map;
	int _row_at_top;
	const LevelState * _state;
};