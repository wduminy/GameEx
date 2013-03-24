/**
 * Copyright 2013 Willem Duminy
 * See LICENCE.txt
 */

#include "main_page.h"

namespace duality {
	MainPage::MainPage() : _font() {}
	
	void MainPage::initialise(const ResourceContext &rc, const DrawContext& dc) {
		_font.reset(new Font("media/duality/comic.ttf", 12, dc.screen()));
	}; 

	void MainPage::update(const UpdateContext &uc) {
	};
	
	void MainPage::draw(const DrawContext &dc) {
		_font->render(100,100,"hello world!");
	};
}