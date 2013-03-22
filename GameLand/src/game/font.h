/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#pragma once
#include <SDL/SDL_ttf.h>
#include <string>
#include "../systemex/systemex.h"
namespace game {

class Font {
	PREVENT_COPY(Font)
public:
	Font(const std::string &fileName, int pointSize);
	~Font();
private:
	TTF_Font * _font;
public:
	typedef std::unique_ptr<Font> u_ptr;
};

}
