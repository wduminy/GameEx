// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0
#include "game.h"
#include <chrono>

namespace codespear {
	Game::Game(const unsigned int window_width, const unsigned int  window_height, const char * window_title)
	 : window_({window_width, window_height},window_title) {
		window_.setVerticalSyncEnabled(true);
	}

	void Game::run()
	{
		while(window_.isOpen())
		{
			auto timePoint1 = std::chrono::high_resolution_clock::now();
			// handle input
			sf::Event event;
			while(window_.pollEvent(event)) {
				if(event.type == sf::Event::Closed) 			{
					window_.close();
					break;
				}
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
				window_.close();
			// update is steps (if drawing is really slow, we catch up)
			current_ += last_;
			for(; current_ >= ftSlice; current_ -= ftSlice)
				update(ftStep);
			// draw
			window_.clear(sf::Color::Black);
			draw(window_);
			window_.display();

			auto timePoint2 = std::chrono::high_resolution_clock::now();
			auto elapsedTime = timePoint2 - timePoint1;
			last_ = std::chrono::duration_cast<
				std::chrono::duration<float, std::milli>>(elapsedTime).count();
		}
	};
}


