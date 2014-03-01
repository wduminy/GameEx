#include "shooter_state.h"

namespace {
	class ShooterCollisions : public game::CollisionListener {
	};

	const game::BoundedBox2 play_box(0,0,LEVEL_WIDTH_PX, LEVEL_HEIGHT_PX);
	
}


ShooterState::ShooterState() : 
	_col_mgr(new ShooterCollisions(),play_box,4),
	_shooter(new Shooter()),
	_map()
	{}

void ShooterState::load_map(const std::string &file_name) {
	_map.reset(new WarZone(file_name));
}

void ShooterState::load_dromes(const std::string &file_name) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(file_name.c_str());
	auto root = doc.RootElement();
	dromes_.load(root->FirstChildElement());
}

void ShooterStateObject::initialise(const game::ResourceContext &rctx, const game::DrawContext &dc) {
	GameObject::initialise(rctx,dc);
	load_map(rctx.path_to("level1.bmp"));
	load_dromes(rctx.path_to("level1.xml"));
}

ShooterStateObject::ShooterStateObject() :
	_left_key_down(false), _right_key_down(false),
	_up_key_down(false), _down_key_down(false)
	{}

void ShooterStateObject::update(const game::UpdateContext & uc) {

	switch (uc.input().key_down()) {
		case SDLK_LEFT:_left_key_down = true;break;
		case SDLK_RIGHT:_right_key_down = true;break;
		case SDLK_UP:_up_key_down = true;break;
		case SDLK_DOWN: _down_key_down = true;break;
		default:break;
	}
	switch (uc.input().key_up()) {
		case SDLK_LEFT:_left_key_down = false;break;
		case SDLK_RIGHT:_right_key_down = false;break;
		case SDLK_UP:_up_key_down = false;break;
		case SDLK_DOWN: _down_key_down = false;break;
		default:break;
	}
	if (_left_key_down && !_right_key_down)
		_shooter->set_dir(HorizontalDir::Left);
	else if (_right_key_down && !_left_key_down)
		_shooter->set_dir(HorizontalDir::Right);
	else
		_shooter->set_dir(HorizontalDir::None);
	if (_up_key_down && !_down_key_down)
		_shooter->set_dir(VerticalDir::Up);
	else if (_down_key_down && !_up_key_down)
		_shooter->set_dir(VerticalDir::Down);
	else
		_shooter->set_dir(VerticalDir::None);
	_shooter->move(uc.seconds_per_update());
}	

