#pragma once
#include <forward_list>
#include <collisions2d.h>
#include <tinyxml2.h>

class Drome : public game::CollidablePolygon  {
protected:
	Drome(const unsigned char type, const tinyxml2::XMLElement * xml);
};

class StaticDrome : public Drome {
public:
	StaticDrome(const tinyxml2::XMLElement * xml);
};

typedef std::forward_list<StaticDrome> StaticDomeList;

class DromeList {
private:
	 StaticDomeList _statics;
public:
	DromeList(const tinyxml2::XMLElement * xml);
	const StaticDomeList & statics() {return _statics;}
};