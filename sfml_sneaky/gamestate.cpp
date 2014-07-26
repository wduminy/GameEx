// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#include "gamestate.h"

namespace codespear {

void State::push(GameState s) {
	m_stack-> push_later(s);
}

void State::switch_to(GameState s) {
	m_stack-> pop_later();
	m_stack->push_later(s);
}

void State::pop() {
	m_stack-> pop_later();
}
void State::clear() {
	m_stack-> clear_later();
}
void StateStack::apply_pendings() {
	for(auto & c : m_pendings) switch(c.op) {
		case Push:
			m_states.push_back(create_state(c.state));
			break;
		case Pop:
			m_states.pop_back();
			break;
		case Clear:
			m_states.clear();
			break;
	}
}
}
