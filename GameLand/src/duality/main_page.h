/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#pragma once
#include "../game/game.h"
#include "../game/font.h"
 
namespace duality {
using namespace game;

class MainPage : public game::GameObjectChainLink {
public:
	MainPage();
	void initialise(const ResourceContext &rc, const DrawContext& dc) override; 
	void update(const UpdateContext &uc) override;
	void draw(const DrawContext &dc) override;
private:
	Font::u_ptr _font;
};
}
