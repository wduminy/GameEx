/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#include "controller.h"
#include "main_scene.h"
#include "front_page.h"
namespace duality {
	DualityController::DualityController() :
		MainObject(-100,DualityScene::NEAREST,DualityScene::FAREST),
		_chain_sentinal(new FrontPage())
	{
		add_part(new SphereCamera(-1, CAM_Y, CAM_RADIUS));
		//add_part(new DualityScene());
		add_part(_chain_sentinal);
	}

	void DualityController::update(const UpdateContext& ctx) {
		if (!_chain_sentinal->is_active())
			deactivate();
		else
			MainObject::update(ctx);
	}

}
