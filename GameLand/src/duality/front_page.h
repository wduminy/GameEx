/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#pragma once
#include "../game/game_objects.h"
#include "../game/font.h"
 
namespace duality {
using namespace game;

class FrontPage : public game::GameObjectChainLink {
public:
	FrontPage();
	void initialise(const ResourceContext &rc, const DrawContext& dc) override; 
	void update(const UpdateContext &uc) override;
	void draw(const DrawContext &dc) override;
private:
	Font2D::u_ptr _font;
	BillBoard _panel;
	ShaderProgram::u_ptr _program;
	Texture::u_ptr _tex;
    Surface::u_ptr _surface;
};
}
