#include <systemex.h>
#include <log.h>
#include "drome.h"
#include "shooter_constants.h"
#include "tileset.h"
using namespace systemex;
using game::Vector2;


Drome::Drome(const object_t type, const tinyxml2::XMLElement * xml, const Shooter& s) : Animate(type), shooter_(s) {
	check_not_null(xml);
	const auto x = xml->IntAttribute("x");
	const auto y = xml->IntAttribute("y");
	const auto mx = LEVEL_WIDTH_PX - STATIC_DOME_PX;
	const auto my = LEVEL_HEIGHT_PX - STATIC_DOME_PX;
	const auto xok = (x >= 0 && x <= mx);
	const auto yok = (y >= 0 && y <= my);
	if (!xok) throw runtime_error_ex("%d is invalid for x. it must be between 0 an %d  ",x, mx);
	if (!yok) throw runtime_error_ex("%d is invalid for y. it must be between 0 an %d", y, my);
	ASSERT(s.map_left()>0);
	set_start(Vector2(x+s.map_left(),y));
	add_relative(Vector2(STATIC_DOME_PX,0));
	add_relative(Vector2(0,STATIC_DOME_PX));
	add_relative(Vector2(-STATIC_DOME_PX,0));
	add_relative(Vector2(0,-STATIC_DOME_PX));
}

void Drome::update(const game::UpdateContext & uc) {
	// TODO 080 update Drome
	//TRACE << "Update" << this;
}

/**
 * The drome is drawn
 * @param dc
 */
void Drome::draw(const game::DrawContext &dc) {
	const auto y = shooter_.xlate_y(box().top(), STATIC_DOME_PX);
	if (y < 0)
		hide();
	TileSet::instance.blit_dome(dc.render(),{box().left(),y});
	// TODO 090 draw Drome
	//TRACE << "Draw";
}
