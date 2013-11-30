#include "shooter_state.h"

namespace {
	class ShooterCollisions : public game::CollisionListener {
	};

	const game::BoundedBox2 play_box(0,0,LEVEL_WIDTH_PX, LEVEL_HEIGHT_PX);
	
	GroundCover cover_from(const unsigned char r, const unsigned char g, const unsigned char b) {
		if (r > 140)
			return GroundCover::Sand;
		if (b > 160)
			return GroundCover::Water;
		if (g > 250)
			return GroundCover::Scrub;
		if (g > 230)
			return GroundCover::Grass;
		if (g > 100)
			return GroundCover::Bush;
		return GroundCover::Road;
	}

}

GroundMap::GroundMap(const std::string &file_name) : _data() {
	game::Surface surface(file_name);
	SDL_Surface& s = surface.sdl();
	if (s.format->BytesPerPixel != 3)
		throw systemex::runtime_error("BMP is not 24 bit.");
	if (s.w != MAP_WIDTH)
		throw systemex::runtime_error_ex("level width (%d) and BMP width (%d) does not match.", MAP_WIDTH, s.w);		
	if (LEVEL_SIZE != static_cast<size_t> (s.w * s.h))
		throw systemex::runtime_error_ex("level size (%d) and BMP size (%d) does not match.", LEVEL_SIZE, s.w * s.h);
	auto source = (unsigned char *) s.pixels;
	for(int c = 0U; c < s.w; c++)
	        for(int r = 0U; r < s.h; r++) {
	        	auto pos = r * s.pitch + (c * 3);
	        	_data[r * s.w + c] = cover_from(source[pos+2],source[pos+1],source[pos]);
	        }
}

Level::Level(const std::string &file_name) : _ground_map(file_name) {}

Shooter::Shooter() : 
	_bottom(LEVEL_HEIGHT_PX-SHOOTER_GUTTER),_left(0),
	_vs(PIXELS_PER_SECOND),_hs(0) {
} 

void Shooter::set_dir(const SteerDir dir) {
	switch (dir) {
		case SteerDir::Left: _hs = -HOR_PIXELS_PER_SECOND;return;
		case SteerDir::Right: _hs = HOR_PIXELS_PER_SECOND;return;
		case SteerDir::None: _hs = 0;return;
	}
} 

void Shooter::move(double lapse) {
	_bottom -= _vs * lapse;
	if (_bottom < SHOOTER_HEIGHT_PX) _bottom = SHOOTER_HEIGHT_PX;
	_left = game::clamp(_left+_hs*lapse,0.0,double(SHOOTER_MAX_LEFT_PX));
}

LevelState::LevelState() : 
	_col_mgr(new ShooterCollisions(),play_box,4),
	_shooter(new Shooter()),
	_level()
	{}

void LevelState::load_level(const std::string &file_name) {
	_level.reset(new Level(file_name));
}

void LevelStateObject::initialise(const game::ResourceContext &rctx, const game::DrawContext &dc) {
	GameObject::initialise(rctx,dc);
	load_level(rctx.path_to("Level1.bmp"));
}

LevelStateObject::LevelStateObject() :
	_left_key_down(false), _right_key_down(false) {}

void LevelStateObject::update(const game::UpdateContext & uc) {

	switch (uc.input().key_down()) {
		case SDLK_LEFT:_left_key_down = true;break;
		case SDLK_RIGHT:_right_key_down = true;break;
		default:break;
	}
	switch (uc.input().key_up()) {
		case SDLK_LEFT:_left_key_down = false;break;
		case SDLK_RIGHT:_right_key_down = false;break;
		default:break;
	}
	if (_left_key_down && !_right_key_down)
		_shooter->set_dir(SteerDir::Left);
	else if (_right_key_down && !_left_key_down)
		_shooter->set_dir(SteerDir::Right);
	else
		_shooter->set_dir(SteerDir::None);
	_shooter->move(uc.seconds_per_update());
}	

