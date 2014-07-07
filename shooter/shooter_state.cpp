#include <log.h>
#include "shooter_state.h"
#include "tileset.h"

namespace {
	class ShooterCollisions : public game::CollisionListener {
	};

	const game::BoundedBox2 play_box(0,0,LEVEL_WIDTH_PX, LEVEL_HEIGHT_PX);

}

ShooterState::ShooterState() :
		game::GameObjectWithDynamicParts(), col_mgr_(new ShooterCollisions(),
				play_box, 4), shooter_(), zone_(), position_schedule_() {
}


void ShooterState::load_dromes(const std::string &file_name) {
	xml_.LoadFile(file_name.c_str());
	auto root = xml_.RootElement();
	auto xml = root->FirstChildElement();
	auto statics = xml->FirstChildElement("staticDromes");
	ASSERT(position_schedule_);
	if (statics != nullptr) {
		auto elem = statics->FirstChildElement();
		while (elem != nullptr) {
			auto add_time = -elem->IntAttribute("y") - WINDOW_HEIGHT ;
			auto remove_time = add_time + WINDOW_HEIGHT*2;
			auto part = new Drome(object_t::StaticDome,elem,*shooter_);
			position_schedule_->push(add_time,[&,part](const game::GameContext& c) {
				add_part(part);
			});
			position_schedule_->push(remove_time,[&,part](const game::GameContext& c) {
				remove_part(part);
			});
			elem = elem->NextSiblingElement();
		}
	}
}

void ShooterState::update(const game::GameContext & c) {
	if (!shooter_) {
		shooter_ = new Shooter((c.d.width()/2) - MAP_WIDTH_PX/2);
			ENSURE(shooter_->map_left() + MAP_WIDTH_PX <= c.d.width(), "map is too wide for screen");
		add_part(zone_ = new WarZone(c.r.path_to("level1.bmp"),*shooter_));
		if (!TileSet::instance.has_image())
			TileSet::instance.load_image(c.r.path_to("Master484.png"),c.d.render());
		zone_->update_tile_indexes();
		add_part(shooter_);
		add_part(position_schedule_ = new PositionSchedule(shooter_));
		load_dromes(c.r.path_to("level1.xml"));
	}
	GameObjectWithDynamicParts::update(c);
}	



