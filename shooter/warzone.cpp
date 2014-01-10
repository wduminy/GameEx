#include "warzone.h"

namespace {
		GroundCover cover_from(const unsigned char r, const unsigned char g, const unsigned char b) {
		if (r > 140)
			return GroundCover::Sand;
		if (b > 160)
			return GroundCover::Water;
		if (g > 250)
			return GroundCover::Scrub;
		if (g > 230)
			return GroundCover::Grass;
		if (g > 100)
			return GroundCover::Bush;
		return GroundCover::Road;
	}
}

WarZone::WarZone(const std::string &file_name) : _data() {
	game::Surface surface(file_name);
	SDL_Surface& s = surface.sdl();
	if (s.format->BytesPerPixel != 3)
		throw systemex::runtime_error("BMP is not 24 bit.");
	if (s.w != MAP_WIDTH)
		throw systemex::runtime_error_ex("level width (%d) and BMP width (%d) does not match.", MAP_WIDTH, s.w);		
	if (LEVEL_SIZE != static_cast<size_t> (s.w * s.h))
		throw systemex::runtime_error_ex("level size (%d) and BMP size (%d) does not match.", LEVEL_SIZE, s.w * s.h);
	auto source = (unsigned char *) s.pixels;
	for(int c = 0U; c < s.w; c++)
	        for(int r = 0U; r < s.h; r++) {
	        	auto pos = r * s.pitch + (c * 3);
	        	_data[r * s.w + c] = cover_from(source[pos+2],source[pos+1],source[pos]);
	        }
}
