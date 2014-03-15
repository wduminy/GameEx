#include <queue>
#include <functional>
#include "game_base.h"
namespace game {

class TimedEvent;

/** a function that takes a GameContext.  Used by TimeHeap */
typedef std::function<void (const game::GameContext)> context_f;

/**
 * Use this GameObject when you want to schedule a context_f to be called
 * at a specific time in the future.
 */
class TimeHeap final : public GameObject {
private:
	std::priority_queue<TimedEvent,std::deque<TimedEvent> > q_;
public:
	/** schedule a function to be executed as a specific time */
	void push(const game_time_t at, context_f fn);
	/** nothing to draw */
	void draw(const DrawContext& gc) final {};
	/** fires an event if it is time */
	void update(const GameContext &c);
};

}
