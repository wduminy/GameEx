#include <log.h>
#include "shooter_state.h"
#include "tileset.h"

namespace {
	class ShooterCollisions : public game::CollisionListener {
	};

	const game::BoundedBox2 play_box(0,0,LEVEL_WIDTH_PX, LEVEL_HEIGHT_PX);
	
}


void ShooterState::load_map(const std::string &file_name) {
	add_part(map_ = new WarZone(file_name,shooter()));
}

void ShooterState::load_dromes(const std::string &file_name) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(file_name.c_str());
	auto root = doc.RootElement();
	auto xml = root->FirstChildElement();
	auto statics = xml->FirstChildElement("staticDromes");
	ASSERT(shooter_);
	if (statics != nullptr) {
		auto elem = statics->FirstChildElement();
		while (elem != nullptr) {
			add_part(new Drome(object_t::StaticDome,elem,*shooter_));
			elem = elem->NextSiblingElement();
		}
	}
}

void ShooterState::initialise(const game::ResourceContext &rctx, const game::DrawContext &dc) {
	shooter_->set_map_left((dc.width()/2) - MAP_WIDTH_PX/2);
	ENSURE(shooter_->map_left() + MAP_WIDTH_PX <= dc.width(), "map is too wide for screen");
	GameObject::initialise(rctx,dc);
	load_map(rctx.path_to("level1.bmp"));
	load_dromes(rctx.path_to("level1.xml"));
	if (!TileSet::instance.has_image())
		TileSet::instance.load_image(rctx.path_to("Master484.png"),dc.render());
	map_->update_tile_indexes();
}

ShooterState::ShooterState() : game::GameObjectWithParts(2),
			col_mgr_(new ShooterCollisions(),play_box,4),
			shooter_(new Shooter()),
			map_(),
	left_key_down_(false), right_key_down_(false),
	up_key_down_(false), down_key_down_(false),
	draw_dst_(),  row_at_top_(LEVEL_HEIGHT-MAP_HEIGHT){
		draw_dst_.x = 0;
		draw_dst_.y = 0;
}

void ShooterState::update(const game::UpdateContext & uc) {

	switch (uc.input().key_down()) {
		case SDLK_LEFT:left_key_down_ = true;break;
		case SDLK_RIGHT:right_key_down_ = true;break;
		case SDLK_UP:up_key_down_ = true;break;
		case SDLK_DOWN: down_key_down_ = true;break;
		default:break;
	}
	switch (uc.input().key_up()) {
		case SDLK_LEFT:left_key_down_ = false;break;
		case SDLK_RIGHT:right_key_down_ = false;break;
		case SDLK_UP:up_key_down_ = false;break;
		case SDLK_DOWN: down_key_down_ = false;break;
		default:break;
	}
	if (left_key_down_ && !right_key_down_)
		shooter_->set_dir(HorizontalDir::Left);
	else if (right_key_down_ && !left_key_down_)
		shooter_->set_dir(HorizontalDir::Right);
	else
		shooter_->set_dir(HorizontalDir::None);
	if (up_key_down_ && !down_key_down_)
		shooter_->set_dir(VerticalDir::Up);
	else if (down_key_down_ && !up_key_down_)
		shooter_->set_dir(VerticalDir::Down);
	else
		shooter_->set_dir(VerticalDir::None);
	shooter_->move(uc.seconds_per_update());
	GameObjectWithParts::update(uc);
}	




void ShooterState::draw(const game::DrawContext &dc) {
	const int bottom_px = shooter().bottom_of_view() + SHOOTER_GUTTER;

	const int top_px = bottom_px - MAP_HEIGHT_PX;

	draw_dst_.x = shooter().map_left() + shooter().box().left();
	draw_dst_.y = shooter().view_top() - top_px;

	TileSet::instance.blit_shooter(dc.render(),draw_dst_);

}


