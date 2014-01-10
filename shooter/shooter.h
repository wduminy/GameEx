#pragma once
#include "shooter_constants.h"
enum class HorizontalDir{Left,Right,None};
enum class VerticalDir{Up,Down,None};

/** Shooter moves from south to north -- decrementing y */
class Shooter {
public:
	Shooter();
	double top() const {return _bottom - _offset - SHOOTER_HEIGHT_PX;}
	double bottom_of_view() const {return _bottom;};
	double left() const {return _left;}
	void move(double lapse);
	void set_dir(const HorizontalDir dir);
	void set_dir(const VerticalDir dir);
private:
	double 
		_offset,  // distance of shooter from bottom
		_bottom,  // latitudinal coordinate of the bottom of the visible map  
		_left,  // distance of shooter from left
		_os,  // offset speed
		_vs, // vertical speed -- pixels per second
		_hs; // horizontal speed -- pixels per second
};
