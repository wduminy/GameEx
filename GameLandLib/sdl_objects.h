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

  /** Encapsulates SDL_Texture */
	class SDLTexture {
		PREVENT_COPY(SDLTexture)
	public:
		explicit SDLTexture(SDL_Texture * texture);
		void blend_mode(SDL_BlendMode mode) {SDL_SetTextureBlendMode(_texture,mode);} 
		virtual ~SDLTexture();
		SDL_Texture * sdl() { return _texture; }
	private:
		SDL_Texture * _texture;
	};

  /** Encapsulates SDL_Renderer */
	class Renderer {
		PREVENT_COPY(Renderer)
	public:
		explicit Renderer(SDL_Window* window);
		void set_logical_size(const int w, const int h) {check(SDL_RenderSetLogicalSize(_renderer,w,h));}
		void copy_from(SDL_Texture* texture,const SDL_Rect* srcrect,const SDL_Rect* dstrect) const;
		void copy_from(SDLTexture& texture,const SDL_Rect& srcrect,const SDL_Rect& dstrect) const {
			copy_from(texture.sdl(),&srcrect,&dstrect);
		}
		void copy_from(SDLTexture& texture) const {
			copy_from(texture.sdl(),0,0);
		}
		void present() {SDL_RenderPresent(_renderer);}
		void clear() {check(SDL_RenderClear(_renderer));}
		SDL_Texture* create_texture(const Uint32 format, const int access, const int w,const int h);
		SDL_Texture* create_texture_from_bmp(const std::string& file_name) const;
		SDL_Texture* create_texture_from_image(const std::string& file_name) const;
		virtual ~Renderer();
	private:
		SDL_Renderer * _renderer;	
	};


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
