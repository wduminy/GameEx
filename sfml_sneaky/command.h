// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0
#pragma once
#include <functional>
#include <queue>
#include <map>
#include "types.h"

namespace codespear {

enum class GameCommand; // to be provided by the specific game

using CommandHandler = std::function<void (FrameTime now)>;

struct CommandEvent {
	GameCommand command;
	FrameTime when;
};

inline bool operator < (const CommandEvent &e1, const CommandEvent &e2) {
	return e1.when > e2.when; // later events have a lower priority
}

class CommandQueue {
private:
	FrameTime m_now;
	std::priority_queue<CommandEvent> m_queue;
	std::map<GameCommand,CommandHandler> m_handlers;
public:
	void register_handler(GameCommand command, CommandHandler handler);
	void schedule(GameCommand command, FrameTime when = 0.f);
	void update(FrameTime dt);
};

}