#pragma once
#include <forward_list>
#include <collisions2d.h>
#include <tinyxml2.h>
enum class game::object_t {Shooter, StaticDome};
using game::object_t;

class Drome : public game::CollidablePolygon  {
public:
	Drome(const object_t type, const tinyxml2::XMLElement * xml);
};


class DromeList : public std::forward_list<Drome> {
public:
	DromeList(const tinyxml2::XMLElement * xml);
};
