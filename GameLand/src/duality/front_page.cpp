/**
 * Copyright 2013 Willem Duminy
 * See LICENCE.txt
 */

#include "front_page.h"

namespace duality {
	FrontPage::FrontPage() : _font(), _panel(-3,3,6,6),_program(), _tex(), _surface() {
	}
	
	void FrontPage::initialise(const ResourceContext &rc, const DrawContext& dc) {
		const char bmpFile[] = "../test.bmp";
//		_surface.reset(new Surface(rc.dir() + bmpFile));
		_surface.reset(new Surface(64));
		_font.reset(new Font2D("media/duality/comic.ttf", 10));
		_program  = rc.load_program(dc.gl(),"panel");
//		_tex.reset(new Texture(dc.gl()));
		// TODO At the moment we need to load the bmp to initialise the texture
		// properly, but we should be able to use the texture without doing this
		_tex = rc.load_texture_bmp(dc.gl(),bmpFile,0);
	}; 

	void FrontPage::update(const UpdateContext &uc) {
		if (uc.input().key_down() == SDLK_ESCAPE)
			deactivate();
	};
	
	void FrontPage::draw(const DrawContext &dc) {
		_font->render(*_surface,0,0,"hello world!");
		_tex->copy_from(*_surface);
	    _tex->activate(_tex->index());
        _program->begin();
        _program->arg("tex",_tex->index());
		glBegin(GL_TRIANGLE_STRIP);
		_panel.draw();
		glEnd();
		_program->end();
	};
}
