/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#include "font.h"

game::Font::Font(const std::string& fileName, int pointSize) :
	_font(TTF_OpenFont(fileName.c_str(),pointSize)) {

}

game::Font::~Font() {
	TTF_CloseFont(_font);
}
