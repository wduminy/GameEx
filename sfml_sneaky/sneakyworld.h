// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0
#pragma once

#include "game.h"
#include "scenenode.h"
namespace sneaky {
using codespear::Game;
using codespear::FrameTime;
using codespear::SceneNode;
using codespear::SceneGraph;
using codespear::GameState;

enum class GameState{Play};
class Arena;
class Head;
class SneakyWorld {

private:
	SceneGraph m_scene_graph;
	Arena * m_arena;
	Head * m_head;
public:
	const SceneNode& scene() const {return m_scene_graph;}
	SneakyWorld();
	void update(FrameTime step);
	void init(sf::Texture &texture);
};

class SneakyGame : public Game {
private:
public:
	SneakyGame();
	void init() override;
};

}
