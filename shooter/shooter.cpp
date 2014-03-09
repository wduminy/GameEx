#include "shooter.h"
#include "shooter_constants.h"
#include <game_math.h>
#include <log.h>
Animate::Animate(const object_t t) : game::CollidablePolygon(t),
	status_(LifeStatus::Dormant), vs_(0),hs_(0) {
} 

Shooter::Shooter(const Scalar& left) :Animate(object_t::StaticDome),
		bottom_(LEVEL_HEIGHT_PX-SHOOTER_GUTTER),
		scroll_speed_(-MIN_SPEED), map_left_(left),
		left_key_down_(false), right_key_down_(false),
		up_key_down_(false), down_key_down_(false),
		row_at_top_(LEVEL_HEIGHT - MAP_HEIGHT),
	 y_speed_(0) {
	move_to({LEVEL_WIDTH_PX/2,bottom_ - SHOOTER_HEIGHT_PX});
}

void Shooter::set_dir(const HorizontalDir dir) {
	switch (dir) {
		case HorizontalDir::Left: hs_ = -HOR_PIXELS_PER_SECOND;return;
		case HorizontalDir::Right: hs_ = HOR_PIXELS_PER_SECOND;return;
		case HorizontalDir::None: hs_ = 0;return;
	}
} 

void Shooter::set_dir(const VerticalDir dir) {
	switch (dir) {
		case VerticalDir::Down: y_speed_ = OFFSET_PIXELS_PER_SECOND;return;
		case VerticalDir::Up: y_speed_ = -OFFSET_PIXELS_PER_SECOND;return;
		case VerticalDir::None: y_speed_ = 0;return;
	}
} 

void Shooter::move(double lapse) {
	vs_ = -game::clamp(-(scroll_speed_ + y_speed_),MIN_SPEED,MAX_SPEED);;
	bottom_ += scroll_speed_ * lapse;
	const auto x = game::clamp(box().left()+ float(hs_*lapse),0.0f,Scalar(SHOOTER_MAX_RIGHT_PX));
	//vs_ = os - scroll_speed_;
	const auto y = box().top()+float((vs_)*lapse);
	move_to({x,y});
	const auto offset_y_ = game::clamp(bottom_-y,0.0f,Scalar(SHOOTER_MAX_OFFSET_PX));
	scroll_speed_ = -game::lerp(offset_y_,0.0f,SHOOTER_MAX_OFFSET_PX,MIN_SPEED,MAX_SPEED);
}


void Shooter::update(const game::GameContext & c) {
	switch (c.u.input().key_down()) {
		case SDLK_LEFT:left_key_down_ = true;break;
		case SDLK_RIGHT:right_key_down_ = true;break;
		case SDLK_UP:up_key_down_ = true;break;
		case SDLK_DOWN: down_key_down_ = true;break;
		default:break;
	}
	switch (c.u.input().key_up()) {
		case SDLK_LEFT:left_key_down_ = false;break;
		case SDLK_RIGHT:right_key_down_ = false;break;
		case SDLK_UP:up_key_down_ = false;break;
		case SDLK_DOWN: down_key_down_ = false;break;
		default:break;
	}
	if (left_key_down_ && !right_key_down_)
		set_dir(HorizontalDir::Left);
	else if (right_key_down_ && !left_key_down_)
		set_dir(HorizontalDir::Right);
	else
		set_dir(HorizontalDir::None);
	if (up_key_down_ && !down_key_down_)
		set_dir(VerticalDir::Up);
	else if (down_key_down_ && !up_key_down_)
		set_dir(VerticalDir::Down);
	else
		set_dir(VerticalDir::None);
	move(c.u.seconds_per_update());
}

Scalar Shooter::xlate_y(const Scalar map_y, const Scalar h) const {
	const auto screen_top = bottom_ - MAP_HEIGHT_PX;
	const auto my_top =  map_y - screen_top;
	return my_top - h;
}


void Shooter::draw(const game::DrawContext &dc) {
	TileSet::instance.blit_shooter(dc.render(),
			map_left() + box().left(),
			xlate_y(box().top(),SHOOTER_HEIGHT_PX));
}
