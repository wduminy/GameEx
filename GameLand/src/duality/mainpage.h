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
	void initialise(const ResourceContext &rctx, const DrawContext& dctx) override; 
	void update(const UpdateContext &context) override;
private:
	Font::u_ptr _font;
};
}
