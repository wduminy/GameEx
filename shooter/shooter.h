#pragma once
#include "shooter_constants.h"
#include <collisions2d.h>
#include "tileset.h"

enum class HorizontalDir{Left,Right,None};
enum class VerticalDir{Up,Down,None};
namespace game {
	enum class object_t {Shooter, StaticDome};
}
using game::object_t;
using game::Scalar;
using game::Vector2;

enum class LifeStatus{Dormant, Active, Destroyed};

class Animate : public game::CollidablePolygon {
private:
	LifeStatus status_;
protected:
	Scalar
		vs_, // vertical speed -- pixels per second
		hs_; // horizontal speed -- pixels per second
	Animate(const object_t t);

};

/** Shooter moves from south to north -- decrementing y */
class Shooter : public Animate {
private:
	Scalar bottom_,    // latitudinal coordinate of the bottom of the visible map
		   os_,        // offset speed
		   map_left_;  // offset of left hand size of visible map
public:
	Shooter();
	Scalar bottom_of_view() const {return bottom_;};
	Scalar view_top() const {return bottom_ - box().top() - SHOOTER_HEIGHT_PX;}
	void move(double lapse);
	void set_dir(const HorizontalDir dir);
	void set_dir(const VerticalDir dir);
	Scalar map_left() const {return map_left_;}
	void set_map_left(const Scalar& v) {map_left_ = v; }
	Scalar xlate_y(const Scalar map_y, const Scalar h) const {return LEVEL_HEIGHT_PX - bottom_ + LEVEL_HEIGHT_PX - map_y -h;}
	//{return bottom_ - (LEVEL_HEIGHT_PX - map_y);}
};
