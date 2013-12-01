/**
 * Copyright 2013 Willem Duminy
 * See LICENCE.txt
 */

#include "font.h"

namespace game {

TTF_Font * open_font(const char * fileName, int pointSize) {
	TTF_Font * result = TTF_OpenFont(fileName, pointSize);
	if (!result)
		throw systemex::runtime_error_ex("open font failed: %s",
				TTF_GetError());
	return result;
}

Font2D::Font2D(const std::string& fileName, int pointSize) :
		_font(open_font(fileName.c_str(), pointSize)), _color( { 255, 0, 0,
				0 }) {
}

void Font2D::render(SDL_Surface& surface, const Sint16 x, const Sint16 y,
		const std::string &text) const {
	SDL_Surface * text_sf = TTF_RenderText_Solid(_font, text.c_str(), _color);
	if (text_sf) {
		SDL_Rect dest_r { x, y, 0, 0 };
		check(SDL_BlitSurface(text_sf, NULL, &surface, &dest_r));
		SDL_FreeSurface(text_sf);
	} else
		throw systemex::runtime_error_ex("render text failed: %s",
				TTF_GetError());
}

void Font2D::render_center(SDL_Surface& surface, const Sint16 x, const Sint16 y,
		const std::string &text) const {
	int w, h;
	TTF_SizeText(_font,text.c_str(),&w,&h);
	render(surface, x - w /2, y,text);
}

Font2D::~Font2D() {
	TTF_CloseFont(_font);
}

void Font::loadChar(int c) {
	GLfloat texcoord[4];
	char letter[2] = { 0, 0 };

	if ((minGlyph <= c) && (c <= maxGlyph) && (NULL == glyphs[c].pic)) {
		SDL_Surface *g0 = NULL;
		SDL_Surface *g1 = NULL;
		letter[0] = c;
		TTF_GlyphMetrics(ttfFont, (Uint16) c, &glyphs[c].minx, &glyphs[c].maxx,
				&glyphs[c].miny, &glyphs[c].maxy, &glyphs[c].advance);
		g0 = TTF_RenderText_Shaded(ttfFont, letter, foreground, background);
		if (g0) {
			g1 = SDL_ConvertSurface(g0,g0->format,0);
			SDL_FreeSurface(g0);
		}
		if (g1) {
			glyphs[c].pic = g1;
			glyphs[c].tex = 0; // loadTextureColorKey(g1, texcoord, 0, 0, 0);
			glyphs[c].texMinX = texcoord[0];
			glyphs[c].texMinY = texcoord[1];
			glyphs[c].texMaxX = texcoord[2];
			glyphs[c].texMaxY = texcoord[3];
		}
	}
}

Font::Font(const char *aaddress, int alength, int apointSize, int astyle,
		float afgRed, float afgGreen, float afgBlue, float abgRed, float abgGreen,
		float abgBlue) :
		height(), ascent(), descent(), lineSkip(), address(
				aaddress), length(alength), pointSize(apointSize), style(astyle), fgRed(
				afgRed), fgGreen(afgGreen), fgBlue(afgBlue), bgRed(abgRed), bgGreen(
				abgGreen), bgBlue(abgBlue), ttfFont(), foreground(), background() {
	int i;

	ttfFont = open_font(address, pointSize);

	TTF_SetFontStyle(ttfFont, style);

	foreground.r = (Uint8) (255 * fgRed);
	foreground.g = (Uint8) (255 * fgGreen);
	foreground.b = (Uint8) (255 * fgBlue);

	background.r = (Uint8) (255 * bgRed);
	background.g = (Uint8) (255 * bgGreen);
	background.b = (Uint8) (255 * bgBlue);

	height = TTF_FontHeight(ttfFont);
	ascent = TTF_FontAscent(ttfFont);
	descent = TTF_FontDescent(ttfFont);
	lineSkip = TTF_FontLineSkip(ttfFont);

	for (i = minGlyph; i <= maxGlyph; i++) {
		glyphs[i].pic = NULL;
		glyphs[i].tex = 0;
	}
}

void Font::textSize(char *text, SDL_Rect *r) {
	int maxx = 0;
	int advance = 0;

	r->x = 0;
	r->y = 0;
	r->w = 0;
	r->h = height;

	while (0 != *text) {
		if ((minGlyph <= *text) && (*text <= maxGlyph)) {
			loadChar(*text);

			maxx = glyphs[(int) *text].maxx;
			advance = glyphs[(int) *text].advance;
			r->w += advance;
		}

		text++;
	}

	r->w = r->w - advance + maxx;
}

void Font::drawText(char *text, int x, int y) {
	GLfloat left, right;
	GLfloat top, bottom;
	GLfloat texMinX, texMinY;
	GLfloat texMaxX, texMaxY;
	GLfloat minx;

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	while (0 != *text) {
		if ((minGlyph <= *text) && (*text <= maxGlyph)) {
			loadChar(*text);

			texMinX = glyphs[((int) *text)].texMinX;
			texMinY = glyphs[((int) *text)].texMinY;
			texMaxX = glyphs[((int) *text)].texMaxX;
			texMaxY = glyphs[((int) *text)].texMaxY;

			minx = glyphs[((int) *text)].minx;

			left = x + minx;
			right = x + glyphs[((int) *text)].pic->w + minx;
			top = y;
			bottom = y + glyphs[((int) *text)].pic->h;

			glBindTexture(GL_TEXTURE_2D, glyphs[((int) *text)].tex);

			glBegin(GL_TRIANGLE_STRIP);

			glTexCoord2f(texMinX, texMinY);
			glVertex2f(left, top);
			glTexCoord2f(texMaxX, texMinY);
			glVertex2f(right, top);
			glTexCoord2f(texMinX, texMaxY);
			glVertex2f(left, bottom);
			glTexCoord2f(texMaxX, texMaxY);
			glVertex2f(right, bottom);

			glEnd();

			x += glyphs[((int) *text)].advance;
		}

		text++;
	}

	glPopAttrib();
}
}
