/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#include "controller.h"
#include "main_scene.h"
Controller::Controller() :
		MainObject(-100, PlayScene::NEAREST, PlayScene::FAREST), _main_page(
				new FrontPage()) {
	auto main_scene = new PlayScene();
	main_scene->set_next(_main_page);
	_main_page->set_next(main_scene);
	add_part(_main_page);
	add_part(main_scene);
}

void Controller::update(const UpdateContext& ctx) {
	if (_main_page->escaped())
		deactivate();
	else
		MainObject::update(ctx);
}
