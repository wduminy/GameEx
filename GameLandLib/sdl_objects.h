/**
 * Copyright 2013 Willem Duminy
 * See LICENCE.txt
 */
#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "systemex.h"
namespace game {
 	void check(int sdl_result);
	void check(void * p);

	class Surface {
		PREVENT_COPY(Surface)
	public:
		explicit Surface(const std::string& path_to_bmp);
		Surface(const int w, const int h);
		operator SDL_Surface& () {return sdl();}
		SDL_Surface& sdl() {return *_surface;}
		~Surface();
	private:
		SDL_Surface * _surface;
    public:
        typedef std::unique_ptr<Surface> u_ptr;
	};
}
