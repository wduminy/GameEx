#include "sdl_objects.h"
#include "systemex.h"
namespace game {
 	void check(int sdl_result) {
		if (sdl_result)
			throw systemex::runtime_error_ex("SDL call returned %d, error:%s",
					sdl_result, SDL_GetError());
	}

	void check(void * p) {
		if (!p)
			throw systemex::runtime_error_ex("SDL error: %s", SDL_GetError());
	}
}

game::Surface::Surface(const std::string& path_to_bmp)
  : _surface(SDL_LoadBMP(path_to_bmp.c_str())) {
	check(_surface);
}

game::Surface::Surface(const int w, const int h)
  :_surface() {
	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    _surface = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32,
                                       rmask, gmask, bmask, amask);
    check(_surface);
}

game::Surface::~Surface() {
	if (_surface)
		SDL_FreeSurface(_surface);
}
