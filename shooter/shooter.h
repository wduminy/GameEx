#pragma once
#include "shooter_constants.h"
#include <collisions2d.h>
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
	Scalar bottom_, // latitudinal coordinate of the bottom of the visible map
		   os_;   // offset speed
public:
	Shooter();
	Scalar bottom_of_view() const {return bottom_;};
	Scalar view_top() const {return bottom_ - box().top() - SHOOTER_HEIGHT_PX;}
	void move(double lapse);
	void set_dir(const HorizontalDir dir);
	void set_dir(const VerticalDir dir);
};
