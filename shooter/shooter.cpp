#include "shooter.h"
#include "shooter_constants.h"
#include <game_math.h>

Animate::Animate(const object_t t) : game::CollidablePolygon(t),
	status_(LifeStatus::Dormant), vs_(MIN_SPEED),hs_(0) {
} 

Shooter::Shooter(const Scalar& left) :Animate(object_t::StaticDome),
		bottom_(LEVEL_HEIGHT_PX-SHOOTER_GUTTER), os_(0), map_left_(left),
		left_key_down_(
				false), right_key_down_(false), up_key_down_(false), down_key_down_(
				false), draw_dst_(), row_at_top_(LEVEL_HEIGHT - MAP_HEIGHT){
	draw_dst_.x = 0;
	draw_dst_.y = 0;
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
		case VerticalDir::Down: os_ = -OFFSET_PIXELS_PER_SECOND;return;
		case VerticalDir::Up: os_ = OFFSET_PIXELS_PER_SECOND;return;
		case VerticalDir::None: os_ = 0;return;
	}
} 

void Shooter::move(double lapse) {
	bottom_ -= vs_ * lapse;
	if (bottom_ < SHOOTER_HEIGHT_PX) bottom_ = SHOOTER_HEIGHT_PX;
	const auto x = game::clamp(box().left()+ float(hs_*lapse),0.0f,Scalar(SHOOTER_MAX_RIGHT_PX));
	const auto y = game::clamp(box().top()+float(os_*lapse),0.0f,Scalar(SHOOTER_MAX_OFFSET_PX));
	move_to({x,y});
	vs_ = game::lerp(box().top(),0.0f,Scalar(SHOOTER_MAX_OFFSET_PX),MIN_SPEED,MAX_SPEED);
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

void Shooter::draw(const game::DrawContext &dc) {
	const int bottom_px = bottom_of_view() + SHOOTER_GUTTER;
	const int top_px = bottom_px - MAP_HEIGHT_PX;
	draw_dst_.x = map_left() + box().left();
	draw_dst_.y = view_top() - top_px;
	TileSet::instance.blit_shooter(dc.render(),draw_dst_);
}
