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
class Arena;

class SneakyWorld {
private:
	SceneGraph m_scene_graph;
	Arena * m_arena;
public:
	const SceneNode& scene() const {return m_scene_graph;}
	SneakyWorld();
};

class SneakyGame : public Game {
private:
	sf::RenderStates m_rstate;
	SneakyWorld m_world;
	sf::View m_view;
public:
	SneakyGame();
	void run() override;
protected:
	void update(FrameTime step) override;
	void draw(sf::RenderTarget &window) override;
};

}
