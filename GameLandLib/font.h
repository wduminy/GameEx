/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#pragma once
#include "gl_objects.h"
#include <SDL2/SDL_ttf.h>
#include <string>
#include "systemex.h"
namespace game {

/* Use  font to render text.
 */
class Font2D {
	PREVENT_COPY(Font2D)
public:
	Font2D(const std::string &fileName, int pointSize);
	/** Set foreground colour. Default is white */
	void set_fg(const SDL_Color &value) {_color = value;} 
	/** Render with transparent background, using the current foreground colour */
	void render(SDL_Surface& surface, const Sint16 x, const Sint16 y, const std::string &text) const;
	/** Draw such that x is the horizontal center */
	void render_center(SDL_Surface& surface, const Sint16 x, const Sint16 y, const std::string &text) const;
	~Font2D();
private:
	TTF_Font * _font;
	SDL_Color _color;
public:
	typedef std::unique_ptr<Font2D> u_ptr;
};

class Font {
	PREVENT_COPY(Font)
public:
	Font(const char *address, int length, int pointSize, int style, float fgRed,
			float fgGreen, float fgBlue, float bgRed, float bgGreen,float bgBlue);
	void drawText(char *text, int x, int y);
private:
	static const int minGlyph = ' ';
	static const int maxGlyph = 126;
	void textSize(char *text, SDL_Rect *r);
	typedef struct {
		int minx, maxx;
		int miny, maxy;
		int advance;
		SDL_Surface *pic;
		GLuint tex;
		GLfloat texMinX, texMinY;
		GLfloat texMaxX, texMaxY;
	} glyph;
	int height;
	int ascent;
	int descent;
	int lineSkip;
	glyph glyphs[maxGlyph + 1];
	const char *address;
	int length;
	int pointSize;
	int style;
	float fgRed, fgGreen, fgBlue;
	float bgRed, bgGreen, bgBlue;
	TTF_Font *ttfFont;
	SDL_Color foreground;
	SDL_Color background;
	void loadChar(int c);
};



}
