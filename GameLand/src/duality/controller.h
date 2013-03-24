/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#pragma once
#include "../game/game.h"
namespace duality {
using namespace game;
class DualityController: public MainObject {
public:
	DualityController();
	void update(const UpdateContext& ctx) override;
private:
	/** The object in the link chain that causes this
	 *  controller to stop when it is not active.
	 */
	GameObject * _chain_sentinal;
};

}
