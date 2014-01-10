#include "shooter.h"
#include "shooter_constants.h"
#include <game_math.h>

Shooter::Shooter() : 
	_offset(0), _bottom(LEVEL_HEIGHT_PX-SHOOTER_GUTTER),_left(0),
	_os(0),
	_vs(MIN_SPEED),_hs(0) {
} 

void Shooter::set_dir(const HorizontalDir dir) {
	switch (dir) {
		case HorizontalDir::Left: _hs = -HOR_PIXELS_PER_SECOND;return;
		case HorizontalDir::Right: _hs = HOR_PIXELS_PER_SECOND;return;
		case HorizontalDir::None: _hs = 0;return;
	}
} 

void Shooter::set_dir(const VerticalDir dir) {
	switch (dir) {
		case VerticalDir::Down: _os = -OFFSET_PIXELS_PER_SECOND;return;
		case VerticalDir::Up: _os = OFFSET_PIXELS_PER_SECOND;return;
		case VerticalDir::None: _os = 0;return;
	}
} 

void Shooter::move(double lapse) {
	_bottom -= _vs * lapse;
	if (_bottom < SHOOTER_HEIGHT_PX) _bottom = SHOOTER_HEIGHT_PX;
	_left = game::clamp(_left+_hs*lapse,0.0,double(SHOOTER_MAX_LEFT_PX));
	_offset = game::clamp(_offset+_os*lapse,0.0,double(SHOOTER_MAX_OFFSET_PX));
	_vs = game::lerp(_offset,0.0,double(SHOOTER_MAX_OFFSET_PX),MIN_SPEED,MAX_SPEED);
}
