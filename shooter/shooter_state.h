#pragma once
#include <game_base.h>
#include <game_math.h>
#include <collisions2d.h>
#include "shooter.h"
#include "warzone.h"
#include "drome.h"
#include "shooter_constants.h"

/** The shooter game state */
class ShooterState : public game::GameObjectWithDynamicParts {
private:
	game::CollisionManagerWithBoxes col_mgr_;
	Shooter * shooter_;
	WarZone * zone_;
public:
	ShooterState();
	void update(const game::GameContext & uc) override;
private:
	void load_dromes(const std::string &file_name);
};
