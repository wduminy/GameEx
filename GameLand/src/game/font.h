/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#pragma once
#include <SDL/SDL_ttf.h>
#include <string>
namespace game {

class Font {
public:
	Font(const std::string &fileName, int pointSize);
	~Font();
private:
	TTF_Font * _font;
};

}
