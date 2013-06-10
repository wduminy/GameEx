/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#include "controller.h"
#include "main_scene.h"
namespace duality {
	DualityController::DualityController() :
		MainObject(-100,DualityScene::NEAREST,DualityScene::FAREST),
		_main_page(new FrontPage())
	{
		auto main_scene = new DualityScene();
		main_scene->set_next(_main_page);
		_main_page->set_next(main_scene);
		add_part(_main_page);
		add_part(main_scene);
	}

	void DualityController::update(const UpdateContext& ctx) {
		if (_main_page->escaped())
			deactivate();
		else
			MainObject::update(ctx);
	}

}
