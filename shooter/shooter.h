#pragma once
#include "shooter_constants.h"
#include <collisions2d.h>
#include <game.h>
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

class Animate : public game::CollidablePolygon,
                public game::GameObject {
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
		   os_;        // offset speed
	const Scalar map_left_;  // offset of left hand size of visible map
	bool left_key_down_, right_key_down_, up_key_down_, down_key_down_;
	SDL_Rect draw_dst_;
	int row_at_top_;
	void move(double lapse);
	void set_dir(const HorizontalDir dir);
	void set_dir(const VerticalDir dir);
	Scalar view_top() const {return bottom_ - box().top() - SHOOTER_HEIGHT_PX;}
public:
	Shooter(const Scalar& left);
	Scalar xlate_y(const Scalar map_y, const Scalar h) const {return LEVEL_HEIGHT_PX - bottom_ + LEVEL_HEIGHT_PX - map_y -h;}
	Scalar map_left() const {return map_left_;}
	Scalar bottom_of_view() const {return bottom_;};
	void update(const game::GameContext & c) override;
	void draw(const game::DrawContext &dc) override;
};
