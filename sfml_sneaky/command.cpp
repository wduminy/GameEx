// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include "command.h"
#include "exception.h"

namespace codespear {

void CommandQueue::register_handler(GameCommand command, CommandHandler handler) {
	m_handlers.emplace(command,handler);
}

void CommandQueue::schedule(GameCommand command, Milliseconds when) {
	m_queue.emplace(CommandEvent{m_handlers.at(command),when});
}

void CommandQueue::schedule(CommandHandler handler, Milliseconds when) {
	m_queue.emplace(CommandEvent{handler,when});
}

void CommandQueue::update(Milliseconds dt) {
	m_now += dt;
	while (!m_queue.empty() && m_queue.top().when <= m_now) {
		auto &h = m_queue.top().handler;
		ASSERT(h); // make sure a handler was registered
		h(m_now);
		m_queue.pop();
	}
}

}
