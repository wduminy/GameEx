#include "drome.h"
#include "shooter_constants.h"
#include <systemex.h>

using namespace systemex;
using game::Vector2;

Drome::Drome(const unsigned char type, const tinyxml2::XMLElement * xml) :
game::CollidablePolygon(type,game::Vector2::origin) {
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

StaticDrome::StaticDrome(const tinyxml2::XMLElement * xml) 
	:	Drome(STATIC_DOME,xml) {
	}

DromeList::DromeList(const tinyxml2::XMLElement * xml) 
	:	_statics() {
				auto statics = xml->FirstChildElement("staticDromes");
		if (statics != nullptr) {
			auto elem = statics->FirstChildElement();
			while (elem != nullptr) {
				_statics.emplace_front(elem);
				elem = elem->NextSiblingElement();
			}
		}
	}	