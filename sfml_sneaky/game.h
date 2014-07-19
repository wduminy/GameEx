// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#pragma once
#include <SFML/Graphics.hpp>
namespace codespear {
using FrameTime = float;

class Game {
private:
	sf::RenderWindow m_window;
	FrameTime m_previous{0.f}, m_current{0.f};
public:
	Game(const unsigned int  window_width, const unsigned int  window_height, const char* window_title);
	virtual void run();
	virtual ~Game() {}
protected:
	virtual void update(FrameTime step) = 0;
	virtual void draw(sf::RenderTarget &window) = 0;
};

}
