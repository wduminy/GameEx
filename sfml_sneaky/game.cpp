// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0
#include "game.h"
#include <chrono>
namespace {
}
namespace codespear {
	Game::Game(const unsigned int window_width, const unsigned int  window_height, const char * window_title)
	 : m_window({window_width, window_height},window_title) {
		m_window.setVerticalSyncEnabled(true);
	}

	void Game::run()
	{
		while(m_window.isOpen())
		{
			auto timePoint1 = std::chrono::high_resolution_clock::now();
			// handle input
			sf::Event event;
			while(m_window.pollEvent(event)) {
				if(event.type == sf::Event::Closed) 			{
					m_window.close();
					break;
				}
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
				m_window.close();
			// update in frame steps
			static const FrameTime frame_step = 1.f;
			m_current += m_previous;
			for(; m_current >= frame_step; m_current -= frame_step)
				update(frame_step);
			// draw
			m_window.clear(sf::Color::Black);
			draw(m_window);
			m_window.display();

			auto timePoint2 = std::chrono::high_resolution_clock::now();
			auto elapsedTime = timePoint2 - timePoint1;
			m_previous = std::chrono::duration_cast<
				std::chrono::duration<float, std::milli>>(elapsedTime).count();
		}
	};
}


