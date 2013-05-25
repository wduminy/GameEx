/**
 * Copyright 2013 Willem Duminy
 * See LICENCE.txt
 */

#include "front_page.h"

namespace duality {
	FrontPage::FrontPage() : _font(), _panel(new Panel(-1,1,2,2)), _escaped(false) {
		add_part(_panel);
	}
	
	void FrontPage::initialise(const ResourceContext &rc, const DrawContext& dc) {
		GameObjectChainLink::initialise(rc,dc);
		_font.reset(new Font2D(rc.dir() +  "comic.ttf", 10));
		_font->render(*_panel,0,0,"hello world!");
	}; 

	void FrontPage::update(const UpdateContext &uc) {
		if (uc.input().key_down() == SDLK_ESCAPE)
			_escaped = true;
		else if (uc.input().key_down() == SDLK_RETURN)
			activate_next();
	};
}
