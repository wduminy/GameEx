/**
 * Copyright 2013 Willem Duminy
 * See LICENCE.txt
 */
#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "systemex.h"
namespace game {
 	void check(int sdl_result);
	void check(void * p);

  /** 
   * A Surface encapsulates an SDL_Surface.
   */
	class Surface {
		PREVENT_COPY(Surface)
	public:
		/** Load surface from BMP file */
		explicit Surface(const std::string& path_to_bmp);
		Surface(const std::string& path_to_bmp, SDL_Surface * compatible);
		Surface(const int w, const int h);
		operator SDL_Surface& () {return sdl();}
		SDL_Surface& sdl() {return *_surface;}
		SDL_Surface * sdl_p() { return _surface; }
		int width() const {return _surface->w; }
		int height() const {return _surface->h; }
		/** Length of the scan line */
		int pitch() const {return _surface->pitch; }
		void set_transparent(const Uint8 r, const Uint8 g, const Uint8 b);
		void write_properties(std::ostream &os);
		/** Copy a rectangle from another surface into this one. 
		 * The width and height in src_rect determine the size of the copied rectangle. 
		 * Only the position is used in the dst_rect (the width and height are ignored). 
		 */
		void blit_from(Surface & src, SDL_Rect & src_rect, SDL_Rect & dst_rect); 
		/** Copy a rectangle from this one surface into another surface one. 
		 * The width and height in src_rect determine the size of the copied rectangle. 
		 * Only the position is used in the dst_rect (the width and height are ignored). 
		 * The way the blitting is done depends on a call to set_transparent().
		 */	
		void blit_to(SDL_Rect & src_rect, SDL_Surface * dst_surface, SDL_Rect & dst_rect); 
		~Surface();
	private:
		SDL_Surface * _surface;
    public:
        typedef std::unique_ptr<Surface> u_ptr;
	};
}
