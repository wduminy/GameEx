#pragma once
#include <game.h>
#include "shooter_constants.h"
#include "shooter.h"

enum class GroundCover {Road,Sand,Water,Grass,Scrub,Bush};  
using game::DrawContext;

class WarZone : public game::GameObject {
public:
	explicit WarZone(const std::string &file_name, const Shooter& s);
	GroundCover operator() (const int x, const int y) const {
		const int ix = y * MAP_WIDTH + x; 
		return (ix < 0 || ix >= LEVEL_SIZE)?GroundCover::Grass:data_[ix];
	} 
	virtual void draw(const DrawContext& gc) override;
	void update_tile_indexes();
private:
	std::array<GroundCover,LEVEL_SIZE> data_;
	SDL_Rect draw_dst_;
	std::array<int,LEVEL_SIZE> tile_x_, tile_y_;
	const Shooter& shooter_;
	void calculate_tile(const int x, const int y, int &t_x, int &t_y);
public:
	typedef std::unique_ptr<WarZone> u_ptr;	
};

