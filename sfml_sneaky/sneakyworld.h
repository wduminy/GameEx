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


class SneakyWorld {
private:
	SceneNode m_scene_graph;
};

class SneakyGame : public Game {
private:
	SneakyWorld m_world;
public:
	SneakyGame();
	void run() override;
protected:
	void update(FrameTime step) override;
	void draw(sf::RenderTarget &window) override;
};

}
