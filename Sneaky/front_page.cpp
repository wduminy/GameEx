/**
 * Copyright 2013 Willem Duminy
 * See LICENCE.txt
 */

#include "front_page.h"

const Scalar PANEL_WIDTH = 1.5; // 2.0 is full screen
const int PANEL_PIX_W = PANEL_WIDTH * 400; // 400 is half of screen pixel size
const Scalar PANEL_HEIGHT = 0.50; // 2.0 is full screen
const int PANEL_PIX_H = PANEL_HEIGHT * 300; // 300 is half of screen pixel size
const int PANEL_CENTER = PANEL_PIX_W / 2;
const int FONT_SIZE = 30;
const int FONT_HEIGHT = 34;

FrontPage::FrontPage() :
		_font(), _panel(
				new Panel(-0.8, 0.8, PANEL_WIDTH, PANEL_HEIGHT, PANEL_PIX_W,
						PANEL_PIX_H)), _escaped(false) {
	add_part (_panel);
	set_active(true);
}

void FrontPage::initialise(const ResourceContext &rc, const DrawContext& dc) {
	GameObjectChainLink::initialise(rc, dc);
	int at = 0;
	_font.reset(new Font2D(rc.dir() + "comic.ttf", FONT_SIZE));
	_font->set_fg( { 28, 57, 187, 0 });
	_font->render_center(*_panel, PANEL_CENTER, 0, "Steer with A and D");
	_font->render_center(*_panel, PANEL_CENTER, at += FONT_HEIGHT,
			"<- and -> rotates the view");
	_font->render_center(*_panel, PANEL_CENTER, at += FONT_HEIGHT,
			"Esc to go away");
	_font->render_center(*_panel, PANEL_CENTER, at += FONT_HEIGHT,
			"Enter to play");
	_panel->update_buffer();

}
;

void FrontPage::update(const GameContext &c) {
	if (c.u.input().key_down() == SDLK_ESCAPE)
		_escaped = true;
	else if (c.u.input().key_down() == SDLK_RETURN)
		activate_next();
}
;
