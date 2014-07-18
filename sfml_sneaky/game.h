// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#pragma once
#include <SFML/Graphics.hpp>
namespace codespear {
constexpr float ftStep{1.f}, ftSlice{1.f};
using FrameTime = float;

class Game {
private:
	sf::RenderWindow window_;
	FrameTime last_{0.f}, current_{0.f};
public:
	Game(const unsigned int  window_width, const unsigned int  window_height, const char* window_title);
	void run();
	virtual ~Game() {}
protected:
	virtual void update(FrameTime step) = 0;
	virtual void draw(sf::RenderTarget &window) = 0;
};

}
