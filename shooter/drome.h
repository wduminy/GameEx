#pragma once
#include <forward_list>
#include <collisions2d.h>
#include <tinyxml2.h>
#include <game.h>
#include "shooter.h"
using game::object_t;

class Drome : public game::GameObject, public Animate {
private:
	const Shooter& shooter_; // Drome needs to be aware of the shooters location
public:
	Drome(const object_t type, const tinyxml2::XMLElement * xml, const Shooter& s);
	void update(const game::GameContext & uc) override;
	void draw(const game::DrawContext &dc) override;
};


