// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#pragma once
#include <SFML/Graphics.hpp>
#include <functional>
#include <map>
#include <memory>
#include <vector>

namespace codespear {
using FrameTime = float;
struct Context : sf::NonCopyable {
	sf::RenderWindow * window;
	sf::Font * font;
	sf::Texture * texture;
};

enum class GameState; // to be provided by the specific game
class StateStack;

class State {
public:
	using u_ptr = std::unique_ptr<State>;
	State(StateStack &stack, Context& context) : m_stack(&stack), m_context(context) {}
	/*
	 * Return true if the event was handled; thus it will not be propagated to
	 * states lower on the stack
	 */
	virtual bool handle_event(const sf::Event&) {return false;}
	virtual void update(FrameTime dt) = 0;
	virtual void draw() = 0;
	virtual ~State() {}
private:
	StateStack * m_stack;
protected:
	Context& m_context;
	void push(GameState s);
	void pop();
	void clear();
};

using StateFactory = std::function<State*(StateStack&,Context&)>;

class StateStack {
public:
	enum Operation {Push,Pop,Clear};
private:
	std::map<GameState,StateFactory> m_factories;
	Context& m_context;
	std::vector<State::u_ptr> m_states;
	struct Pending {Operation op; GameState state;};
	std::vector<Pending> m_pendings;
public:
	explicit StateStack(Context& context) : m_context(context) {}
	void register_state(GameState state, StateFactory f) {m_factories[state] = f;}
	template <typename T> void register_state(GameState state) {
		register_state(state,
				[&](StateStack&s,Context& c)
					{return new T(*this,m_context);}
		);
	}
	State::u_ptr create_state(GameState state) {
		auto& fnd = m_factories.at(state);
		return State::u_ptr(fnd(*this,m_context));
	}
	void handle_event(const sf::Event& event) {
		for (auto i = m_states.rbegin(); i != m_states.rend(); ++i)
			if ((*i)->handle_event(event))
				return;
		apply_pendings();
	}
	void update(FrameTime delta) {
		for (auto i = m_states.rbegin(); i != m_states.rend(); ++i)
			(*i)->update(delta);
	}
	void draw() {
		for (auto i = m_states.begin(); i != m_states.end(); ++i)
			(*i)->draw();
	}
	void push(GameState s) {m_states.push_back(create_state(s));}
	void pop() { m_states.pop_back(); }
	void clear() { m_states.clear(); }
	bool is_empty() const { return m_states.size() == 0; }
	void push_later(GameState s) {m_pendings.push_back({Push,s});}
	void pop_later() { m_pendings.push_back({Pop,GameState()}); }
	void clear_later() { m_pendings.push_back({Clear,GameState()}); }
private:
	void apply_pendings();
};

}
