// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#pragma once
#include "gamestate.h"
namespace codespear {


class Game {
private:
	sf::RenderWindow m_window;
	StateStack m_stack;
	FrameTime m_previous{0.f}, m_current{0.f};
	Context m_context;
public:
	Game(const unsigned int  window_width, const unsigned int  window_height, const char* window_title);
	void run();
	virtual ~Game() {}
protected:
	const sf::RenderWindow& window() const {return m_window;}
	virtual void init() {};
	virtual void update(FrameTime step) = 0;
	virtual void draw(sf::RenderTarget &window) = 0;
};

}
