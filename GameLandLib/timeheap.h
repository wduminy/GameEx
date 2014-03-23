#include <queue>
#include <functional>
#include "game_base.h"
namespace game {

/** a function that takes a GameContext.  Used by TimeHeap */
typedef std::function<void (const game::GameContext)> context_f;

template<typename T> class FutureEvent {
public:
	FutureEvent(const T t, context_f fn) : fire_at_(t), fn_(fn) {}
	void fire(const GameContext& c) const {fn_(c);};
	int fire_point() const {return fire_at_;}
	bool operator >(const FutureEvent<T> &o) const {return fire_at_ > o.fire_at_;}
private:
	T fire_at_;
	context_f fn_;
};

/**
 * Use this GameObject when you want to schedule a context_f to be called
 * at a specific point.  The point is identified by an class parameter.
 */
template <class T> class EventHeap  : public GameObject {
private:
	typedef FutureEvent<T> event_t;
	std::priority_queue<event_t,std::deque<event_t>,std::greater<event_t> > q_;
public:
	void push(const int at, context_f fn) {
		q_.push(event_t(at,fn));
	}
	/** nothing to draw */
	void draw(const DrawContext& gc) final {};
	/** fires an event if it there is one at or less than than this point */
	void update(const GameContext &c) final {
		const auto n = now(c);
		while (!q_.empty() && (q_.top().fire_point() <= n)) {
			q_.top().fire(c);
			q_.pop();
		}
		GameObject::update(c);
	}
	/** called during update to determine the current point */
	virtual T now(const GameContext &c) const = 0;
	virtual ~EventHeap(){}
};


/**
 * Use this GameObject when you want to schedule a context_f to be called
 * at a specific time in the future.
 */
class TimeHeap final : public EventHeap<game_time_t> {
public:
	game_time_t now(const GameContext &c) const {return c.u.time();}
};





}
