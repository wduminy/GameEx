#pragma once
#include <forward_list>
#include <collisions2d.h>
#include <tinyxml2.h>
#include <game.h>
#include "shooter.h"
using game::object_t;

class Drome : public Animate  {
public:
	Drome(const object_t type, const tinyxml2::XMLElement * xml);
};


class DromeList : public std::forward_list<Drome> {
public:
	void load(const tinyxml2::XMLElement * xml);
};


class DromeObject : public game::GameObject {
private:
	Drome & drome_;
	const Shooter& shooter_; // Drome needs to be aware of the shooters location
public:
	DromeObject(Drome& d, const Shooter& s) : drome_(d), shooter_(s) {}
};
