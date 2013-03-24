/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#pragma once
#include "sdl_utils.h" 
#include <SDL/SDL_ttf.h>
#include <string>
#include "../systemex/systemex.h"
namespace game {

/* Use a font to render text. 
 */
class Font {
	PREVENT_COPY(Font)
public:
	Font(const std::string &fileName, int pointSize, SDL_Surface * surface);
	/** Set foreground colour. Default is white */
	void set_fg(const SDL_Color &value) {_color = value;} 
	/** Render with transparent background */
	void render(const Sint16 x, const Sint16 y, const std::string &text) const;
	~Font();
private:
	TTF_Font * _font;
	SDL_Color _color;
	SDL_Surface * _surface;
public:
	typedef std::unique_ptr<Font> u_ptr;
};

}
