#pragma once
#include <game_base.h>
#include <collisions2d.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
// number of tiles in a row (based on xevious estimation)
#define MAP_WIDTH 15 
// number of rows in a level
#define LEVEL_HEIGHT 137  
// number of pixels in a tile side (it is a square)
#define TILE_SIZE_PX 58 
#define LEVEL_WIDTH_PX MAP_WIDTH*TILE_SIZE_PX
#define LEVEL_HEIGHT_PX LEVEL_HEIGHT*TILE_SIZE_PX
#define SHOOTER_WIDTH_PX 30*2
#define SHOOTER_HEIGHT_PX 18*2
// the shooter is never lower than this point
#define SHOOTER_GUTTER TILE_SIZE_PX*2
// the number of seconds to scroll through a level
#define SECONDS_PER_LEVEL 20
#define PIXELS_PER_SECOND LEVEL_HEIGHT_PX/SECONDS_PER_LEVEL 

#define LEVEL_SIZE MAP_WIDTH*LEVEL_HEIGHT

enum class GroundCover {Road,Sand,Water,Grass,Scrub,Bush};  

class GroundMap {
public:
	explicit GroundMap(const std::string &file_name);
	GroundCover operator() (const int x, const int y) const {
		const int ix = y * MAP_WIDTH + x; 
		return (ix < 0 || ix >= LEVEL_SIZE)?GroundCover::Grass:_data[ix];
	} 
private:
	std::array<GroundCover,LEVEL_SIZE> _data;
public:
	typedef std::unique_ptr<GroundMap> u_ptr;	
};

class Level {
	public:
		explicit Level(const std::string &file_name);
		const GroundMap& ground() const {return _ground_map;}
	private:	
		GroundMap _ground_map;
};

/** Shooter moves from south to north -- decrementing y */
class Shooter {
public:
	Shooter();
	double bottom() const {return _bottom;};
	void move(double lapse) {
			_bottom -= _speed * lapse;
			if (_bottom < SHOOTER_HEIGHT_PX) _bottom = SHOOTER_HEIGHT_PX;
	}
private:
	double _bottom;
	double _speed;
};

class LevelState {
public:	
	LevelState();
	const Shooter& shooter() const {return *_shooter; }
	const Level& level() const {ASSERT(_level); return *_level;}
protected:
	void load_level(const std::string &file_name);
	game::CollisionManagerWithBoxes _col_mgr;
	std::unique_ptr<Shooter> _shooter;
	std::unique_ptr<Level> _level;
};

class LevelStateObject : public game::GameObject, public LevelState {
public:
	void initialise(const game::ResourceContext &rctx, const game::DrawContext &dc) override;
	void update(const game::UpdateContext & uc) override;
	void draw(const game::DrawContext &dc) override {};
};