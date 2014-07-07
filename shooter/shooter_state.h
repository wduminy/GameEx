#pragma once
#include <game_base.h>
#include <game_math.h>
#include <collisions2d.h>
#include "shooter.h"
#include "warzone.h"
#include "drome.h"
#include "shooter_constants.h"
#include <timeheap.h>

/* A EventHeap that is based on the position of the shooter */
class PositionSchedule final : public game::EventHeap<game::Scalar> {
private:
	Shooter * shooter_;
public:
	PositionSchedule(Shooter * s) : shooter_(s) {};
	game::Scalar now(const game::GameContext &c) const override {
		return -shooter_->box().top();
	}

};

/** The shooter game state */
class ShooterState : public game::GameObjectWithDynamicParts {
private:
	game::CollisionManagerWithBoxes col_mgr_;
	Shooter * shooter_;
	WarZone * zone_;
	tinyxml2::XMLDocument xml_;
	PositionSchedule * position_schedule_;
public:
	ShooterState();
	void update(const game::GameContext & uc) override;
private:
	void load_dromes(const std::string &file_name);
};
