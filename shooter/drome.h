#pragma once
#include <forward_list>
#include <collisions2d.h>
#include <tinyxml2.h>
#include "shooter.h"
using game::object_t;

class Drome : public Animate  {
public:
	Drome(const object_t type, const tinyxml2::XMLElement * xml);
};


class DromeList : public std::forward_list<Drome> {
public:
	DromeList(const tinyxml2::XMLElement * xml);
};
