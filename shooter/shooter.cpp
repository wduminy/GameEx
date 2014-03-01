#include "shooter.h"
#include "shooter_constants.h"
#include <game_math.h>

Animate::Animate(const object_t t) : game::CollidablePolygon(t),
	status_(LifeStatus::Dormant), vs_(MIN_SPEED),hs_(0) {
} 

Shooter::Shooter() :Animate(object_t::StaticDome),
		bottom_(LEVEL_HEIGHT_PX-SHOOTER_GUTTER), os_(0) {}

void Shooter::set_dir(const HorizontalDir dir) {
	switch (dir) {
		case HorizontalDir::Left: hs_ = -HOR_PIXELS_PER_SECOND;return;
		case HorizontalDir::Right: hs_ = HOR_PIXELS_PER_SECOND;return;
		case HorizontalDir::None: hs_ = 0;return;
	}
} 

void Shooter::set_dir(const VerticalDir dir) {
	switch (dir) {
		case VerticalDir::Down: os_ = -OFFSET_PIXELS_PER_SECOND;return;
		case VerticalDir::Up: os_ = OFFSET_PIXELS_PER_SECOND;return;
		case VerticalDir::None: os_ = 0;return;
	}
} 

void Shooter::move(double lapse) {
	bottom_ -= vs_ * lapse;
	if (bottom_ < SHOOTER_HEIGHT_PX) bottom_ = SHOOTER_HEIGHT_PX;
	const auto x = game::clamp(left()+ float(hs_*lapse),0.0f,Scalar(SHOOTER_MAX_LEFT_PX));
	const auto y = game::clamp(top()+float(os_*lapse),0.0f,Scalar(SHOOTER_MAX_OFFSET_PX));
	move_to({x,y});
	vs_ = game::lerp(top(),0.0f,Scalar(SHOOTER_MAX_OFFSET_PX),MIN_SPEED,MAX_SPEED);
}
