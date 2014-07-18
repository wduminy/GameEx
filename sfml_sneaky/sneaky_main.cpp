// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include "game.h"

struct Game : public codespear::Game
{
	Game() : codespear::Game(800, 600, "Sneaky") {

	}

	void update(codespear::FrameTime ftStep) override {
	}

	void draw(sf::RenderTarget & window) override {
	}
};

int main()
{
	Game{}.run(); return 0;
}
