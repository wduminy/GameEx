/**
 * Copyright 2013 Willem Duminy
 * See LICENCE.txt
 */

#include "front_page.h"

namespace duality {
	FrontPage::FrontPage() : _font(),
			_panel(new Panel(-0.8,0.8,0.5,0.5,160,80)), _escaped(false) {
		add_part(_panel);
		set_active(true);
	}
	
	void FrontPage::initialise(const ResourceContext &rc, const DrawContext& dc) {
		GameObjectChainLink::initialise(rc,dc);
		auto lh = 20;
		_font.reset(new Font2D(rc.dir() +  "comic.ttf", 14));
		_font->render(*_panel,0,0,"Press enter to start");
		_font->render(*_panel,0,lh,"Press escape to exit");
		_panel->update_buffer();

	}; 

	void FrontPage::update(const UpdateContext &uc) {
		if (uc.input().key_down() == SDLK_ESCAPE)
			_escaped = true;
		else if (uc.input().key_down() == SDLK_RETURN)
			activate_next();
	};
}
