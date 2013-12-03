#include "sdl_objects.h"
#include <SDL2/SDL_image.h>
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

namespace {
  SDL_Surface * load_and_convert(const char * path, SDL_Surface * compatible) {
    SDL_Surface * temp = SDL_LoadBMP(path);
    SDL_Surface * result = SDL_ConvertSurface(temp,compatible->format, SDL_SWSURFACE);
    SDL_FreeSurface(temp);
    return result;
  }
}

namespace game {
  SDLTexture::SDLTexture(SDL_Texture * texture) : _texture(texture) {
    ASSERT(texture);
  }

  SDLTexture::~SDLTexture() {
    SDL_DestroyTexture(_texture);
  }

  Renderer::Renderer(SDL_Window * window): _renderer(0) {
    SDL_Renderer * r = SDL_CreateRenderer(window,-1,0);
    check(r);
    _renderer = r;
  }

  void Renderer::copy_from(SDL_Texture* texture,
    const SDL_Rect* srcrect,const SDL_Rect* dstrect) const {
    check(SDL_RenderCopy(_renderer, texture, srcrect, dstrect));
  }

  SDL_Texture* Renderer::create_texture(const Uint32 format,const int access,
    const int w,const int h) {
    auto r = SDL_CreateTexture(_renderer,format,access,w,h);
    check(r);
    return r;
  }

  SDL_Texture* Renderer::create_texture_from_bmp(const std::string& file_name) const {
    auto surface = SDL_LoadBMP(file_name.c_str());
    check(surface);
    auto r = SDL_CreateTextureFromSurface(_renderer, surface);
    SDL_FreeSurface(surface);
    return r;
  }

  SDL_Texture* Renderer::create_texture_from_image(const std::string& file_name) const {
    auto surface = IMG_Load(file_name.c_str());
    check(surface);
    auto r = SDL_CreateTextureFromSurface(_renderer, surface);
    SDL_FreeSurface(surface);
    return r;
  }

  Renderer::~Renderer() {
    if (_renderer)
      SDL_DestroyRenderer(_renderer);
   } 
}

game::Surface::Surface(const std::string& path_to_bmp)
  : _surface(SDL_LoadBMP(path_to_bmp.c_str())) {
	check(_surface);
}

game::Surface::Surface(const std::string& path_to_bmp, SDL_Surface * compatible)
  : _surface(load_and_convert(path_to_bmp.c_str(),compatible)) {
  check(_surface);
}

void game::Surface::blit_from(Surface & src, SDL_Rect & src_rect, SDL_Rect & dst_rect) {
  check(SDL_BlitSurface(src.sdl_p(),&src_rect,_surface,&dst_rect));
}

void game::Surface::blit_to(SDL_Rect & src_rect, SDL_Surface * dst_surface, SDL_Rect & dst_rect) {
  check(SDL_BlitSurface(_surface,&src_rect,dst_surface,&dst_rect));
}


void game::Surface::set_transparent(const Uint8 r, const Uint8 g, const Uint8 b) {
  auto colorKey = SDL_MapRGB(_surface->format,r,g,b);
  check(SDL_SetColorKey(_surface,SDL_TRUE,colorKey));
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

void game::Surface::write_properties(std::ostream &os) {
  os << "width=" << _surface->w << " height=" << _surface->h  << " pitch=" << _surface->pitch;
}

game::Surface::~Surface() {
	if (_surface)
		SDL_FreeSurface(_surface);
}
