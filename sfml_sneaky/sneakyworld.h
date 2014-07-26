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

enum class GameState{Title,Play};


class SneakyGame : public Game {
private:
	sf::Texture m_texture;
	sf::Font m_font;
public:
	SneakyGame();
	void init() override;
};

}
