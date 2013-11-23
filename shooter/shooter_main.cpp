#include <game_base.h>
#include <log.h>
#include "shooter_map.h"

namespace {
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;
	const float WH_RATIO = WINDOW_WIDTH * 1.0f / WINDOW_HEIGHT;
}

class ShooterController : public game::MainObject {
	public:
	ShooterController() : game::MainObject() {
		add_part(new ShooterMap());
	}
};



int main( int count, char* args[] ) {
	LOG << "started shooter";
	try {
		bool fullscreen = false;
		if (count > 1) {
			const std::string a = args[1];
			fullscreen = a == "-fullscreen";
		}
		game::Game shooter(new ShooterController(), "shooter/", 
			20, // ups 
			20, // fps
			fullscreen, 
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			false // no opengl
			);
		return shooter.run();
	} catch (std::exception &e) {
		LOG << "exception: " << (e.what()) << std::endl;
        return EXIT_FAILURE;
  	} catch (...) {
		LOG << "unexpected error occurred";
        return EXIT_FAILURE;
    }
	return EXIT_SUCCESS;
}