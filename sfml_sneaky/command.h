// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0
#pragma once
#include <functional>
#include <queue>
#include <map>
#include "types.h"
#include <mutex>

namespace codespear {
using guard = std::lock_guard<std::mutex>;

enum class GameCommand; // to be provided by the specific game

using CommandHandler = std::function<void (Milliseconds now)>;

struct CommandEvent {
	CommandHandler handler;
	Milliseconds when;
};

inline bool operator < (const CommandEvent &e1, const CommandEvent &e2) {
	return e1.when > e2.when; // later events have a lower priority
}

class CommandQueue {
private:
	Milliseconds m_now;
	std::priority_queue<CommandEvent> m_queue;
	std::map<GameCommand,CommandHandler> m_handlers;
public:
	void register_handler(GameCommand command, CommandHandler handler);
	void schedule(GameCommand command, Milliseconds when = 0.f);
	void schedule(CommandHandler command, Milliseconds when = 0.f);
	void update(Milliseconds dt);
};

/* use this if your game has multiple thread that inject into the queue
 * alias: CommandQueueThreadSafe
 */
class CommandQueueTS {
public:
	void register_handler(GameCommand command, CommandHandler handler) {
		guard g(m_mtx);
		m_q.register_handler(command,handler);
	}
	void schedule(CommandHandler command, Milliseconds when = 0.f) {
		guard g(m_mtx);
		m_q.schedule(command,when);
	}
	// update is typically called from one controlling thread
	void update(Milliseconds dt) {
		guard g(m_mtx);
		m_q.update(dt);
	}
private:
	std::mutex m_mtx;

	CommandQueue m_q;
};

};
