/**
 * Copyright 2013 Willem Duminy
 * See LICENCE.txt
 */

#include "font.h"

namespace game {

	TTF_Font * open_font(const char * fileName, int pointSize) {
		TTF_Font * result = TTF_OpenFont(fileName,pointSize);
		if (!result)
			throw systemex::runtime_error_ex("open font failed: %s", TTF_GetError());
		return result;
	}

	Font::Font(const std::string& fileName, int pointSize, SDL_Surface * surface) :
		_font(open_font(fileName.c_str(),pointSize)),
		_color({255,0,0,128}),
		_surface(surface) {}
		
	void Font::render(const Sint16 x, const Sint16 y, const std::string &text) const {
		SDL_Surface * text_sf = TTF_RenderText_Solid(_font,text.c_str(),_color);
		if (text_sf) {
			SDL_Rect dest_r = {x,y,0,0};
			check(SDL_BlitSurface(text_sf,NULL,_surface,&dest_r));
			SDL_FreeSurface(text_sf);
		} else
			throw systemex::runtime_error_ex("render text failed: %s", TTF_GetError());
	}

	Font::~Font() {
		TTF_CloseFont(_font);
	}

}
