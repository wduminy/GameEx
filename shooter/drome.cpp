#include "drome.h"
#include <systemex.h>
#include "shooter_constants.h"

using namespace systemex;
using game::Vector2;


Drome::Drome(const object_t type, const tinyxml2::XMLElement * xml, const Shooter& s) : Animate(type), shooter_(s) {
	check_not_null(xml);
	const auto x = xml->FloatAttribute("x");
	const auto y = xml->FloatAttribute("y");
	check_that(x > 0 && x < LEVEL_WIDTH_PX - DROME_WIDTH_PX);
	check_that(y > 0 && y < LEVEL_HEIGHT_PX - DROME_HEIGHT_PX);
	set_start(Vector2(x,y)); 
	add_relative(Vector2(DROME_WIDTH_PX,0));
	add_relative(Vector2(0,DROME_HEIGHT_PX));
	add_relative(Vector2(-DROME_WIDTH_PX,0));
	add_relative(Vector2(0,-DROME_HEIGHT_PX));
}

void Drome::update(const game::UpdateContext & uc) {
	// TODO 080 draw Drome
}

void Drome::draw(const game::DrawContext &dc) {
	// TODO 090 draw Drome
}
