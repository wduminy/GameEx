/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#include "heightmap.h"
using namespace game;

namespace terrain {
void read_heightmap(const string& filename,size_t buffer_size, unsigned char * buffer) {
	Surface surface(filename);
	SDL_Surface& s = surface.sdl();
	if (s.format->BytesPerPixel != 3)
		throw runtime_error("BMP is not 24 bit.");
	if (buffer_size != static_cast<size_t> (s.w * s.h))
		throw runtime_error("buffer and BMP size does not match.");
	auto source = (unsigned char *) s.pixels;
	for(int c = 0U; c < s.w; c++)
	        for(int r = 0U; r < s.h; r++) {
	        	auto pos = r * s.pitch + (c * 3);
	        	buffer[r * s.w + c] = source[pos];
	        }
}
void write_heightmap(const string& filename,int width, int height, unsigned char * buffer) {
	SDL_Surface * s = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 24, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);
	auto target = (unsigned char *) s->pixels;
    for(int c = 0U; c < width; c++)
        for(int r = 0U; r < height; r++) {
        	auto pos = r * s->pitch + (c * 3);
        	target[pos] = target[pos+1] = target[pos+2] = buffer[r * width + c];
        }
	SDL_SaveBMP(s, filename.c_str());
	SDL_FreeSurface(s);

}
}
