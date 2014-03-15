#include "timeheap.h"

namespace game {

class TimedEvent {
public:
	TimedEvent(const game_time_t t, context_f fn) : fire_at_(t), fn_(fn) {}
	void fire(const GameContext& c) const {fn_(c);};
	game_time_t fire_time() const {return fire_at_;}
	bool operator <(const TimedEvent &o) const {return fire_at_ < o.fire_at_;}
private:
	game_time_t fire_at_;
	context_f fn_;
};

void TimeHeap::push(const game_time_t at, context_f fn) {
	q_.push(TimedEvent(at,fn));
}

void TimeHeap::update(const GameContext &c) {
	while (!q_.empty() && (q_.top().fire_time() <= c.u.time())) {
		q_.top().fire(c);
		q_.pop();
	}
	GameObject::update(c);
}

}
