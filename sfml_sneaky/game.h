// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#pragma once
#include "gamestate.h"
namespace codespear {


class Game {
private:
	sf::RenderWindow m_window;
	FrameTime m_previous{0.f}, m_current{0.f};
public:
	Game(const unsigned int  window_width,
			const unsigned int  window_height,
			const char* window_title);
	void run();
	virtual ~Game() {}
protected:
	Context m_context;
	StateStack m_stack;
	virtual void init() {};
};

}
