// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0
#include "game.h"
#include <chrono>
#include <iostream>
namespace codespear {


	Game::Game(const unsigned int window_width,
			const unsigned int  window_height,
			const char * window_title)
	 : m_window({window_width, window_height},window_title),
	   m_context(),
	   m_stack(m_context){
		m_window.setVerticalSyncEnabled(true);
		m_context.window = &m_window;
	}

	void Game::run()
	{
		try {
			init();
			m_stack.push(GameState()); // start with the first game state
			while(m_window.isOpen())
			{
				auto timePoint1 = std::chrono::high_resolution_clock::now();
				// handle input
				sf::Event event;
				while(m_window.pollEvent(event)) {
					if(event.type == sf::Event::Closed) 			{
						m_stack.clear();
					} else
						m_stack.handle_event(event);
				}
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
					m_window.close();
				else {
					// update in frame steps
					static const FrameTime frame_step = 50.f; // 50 millis is 20 updates per second
					m_current += m_previous;
					for(; m_current >= frame_step; m_current -= frame_step)
						m_stack.update(frame_step);
					if (m_stack.is_empty())
						m_window.close();
					else {
						// draw
						m_window.clear(sf::Color::Black);
						m_stack.draw();
						m_window.display();

						auto timePoint2 = std::chrono::high_resolution_clock::now();
						auto elapsedTime = timePoint2 - timePoint1;
						m_previous = std::chrono::duration_cast<
							std::chrono::duration<float, std::milli>>(elapsedTime).count();
					}
				}
			}
		}
		catch (std::exception &e) {
			m_window.close();
			std::cerr << "Error on run:" << e.what();
		}
	};
}


