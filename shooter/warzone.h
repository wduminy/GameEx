#pragma once
#include <game.h>
#include "shooter_constants.h"

enum class GroundCover {Road,Sand,Water,Grass,Scrub,Bush};  

class WarZone {
public:
	explicit WarZone(const std::string &file_name);
	GroundCover operator() (const int x, const int y) const {
		const int ix = y * MAP_WIDTH + x; 
		return (ix < 0 || ix >= LEVEL_SIZE)?GroundCover::Grass:_data[ix];
	} 
private:
	std::array<GroundCover,LEVEL_SIZE> _data;
public:
	typedef std::unique_ptr<WarZone> u_ptr;	
};

