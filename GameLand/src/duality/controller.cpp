/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#include "controller.h"
#include "main_scene.h"
#include "main_page.h"
namespace duality {
	DualityController::DualityController() :
		MainObject(-100,DualityScene::NEAREST,DualityScene::FAREST) 
	{
		add_part(GameObject::u_ptr(new DualityScene()));
//		add_part(GameObject::u_ptr(new MainPage()));
	}

}
