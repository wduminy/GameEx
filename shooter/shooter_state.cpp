#include "shooter_state.h"

namespace {
	class ShooterCollisions : public game::CollisionListener {
	};

	const game::BoundedBox2 play_box(0,0,LEVEL_WIDTH_PX, LEVEL_HEIGHT_PX);

}

Shooter::Shooter() : 
	_bottom(LEVEL_HEIGHT_PX-SHOOTER_GUTTER),
	_speed(PIXELS_PER_SECOND) {
} 

LevelState::LevelState() : 
	_col_mgr(new ShooterCollisions(),play_box,4),
	_shooter(new Shooter())
	{}

void LevelStateObject::update(const game::UpdateContext & uc) {
	_shooter->move(uc.seconds_per_update());
}	