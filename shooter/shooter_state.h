#pragma once
#include <game_base.h>
#include <game_math.h>
#include <collisions2d.h>
#include "shooter.h"
#include "warzone.h"
#include "shooter_constants.h"

/* The container of the game model */
class ShooterState {
public:	
	ShooterState();
	const Shooter& shooter() const {return *_shooter; }
	const WarZone& map() const {ASSERT(_map); return *_map;}
protected:
	void load_map(const std::string &file_name);
	game::CollisionManagerWithBoxes _col_mgr;
	std::unique_ptr<Shooter> _shooter;
	std::unique_ptr<WarZone> _map;
};

class ShooterStateObject : public game::GameObject, public ShooterState {
public:
	ShooterStateObject();
	void initialise(const game::ResourceContext &rctx, const game::DrawContext &dc) override;
	void update(const game::UpdateContext & uc) override;
private:
	bool _left_key_down, _right_key_down, _up_key_down, _down_key_down;	
};