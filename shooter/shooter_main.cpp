#include <game_base.h>
#include <log.h>
#include "shooter_state.h"
#include <tut/tut.hpp>
#include <tut/tut_reporter.hpp>

class ShooterController : public game::MainObject {
	public:
	ShooterController() : game::MainObject() {
		add_part(new ShooterState());
	}
};

void run_tests() {
	tut::console_reporter reporter(systemex::Log::instance().file());
	tut::runner.get().set_callback(&reporter);
	tut::runner.get().run_tests();
	if (reporter.all_ok())
		LOG << "Unit tests passed";
	else
		LOG << "Unit tests failed!";
}

class Args {
private:
	char ** _args;
	int _count;
public:
	Args(int count, char ** args) : _args(args),_count(count) {};
	bool contains(const std::string &v) const {
		for (int i=0;i<_count;i++)
			if (v == _args[i])
				return true;
		return false;	
	}	
};

int main( int count, char* args[] ) {
	try {
		Args runArgs(count,args);
		if (runArgs.contains("-test")) {
			run_tests();
			return 0;
		}
		LOG << "started shooter";
		const bool fullscreen = runArgs.contains("-fullscreen");
		game::Game shooter(new ShooterController(), "shooter/", 
			30, // ups 
			30, // fps
			fullscreen, 
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			false // no opengl
			);
		return shooter.run();
	} catch (std::exception &e) {
		LOG << "exception: " << (e.what()) << std::endl;
		std::cout << "exception: " << (e.what()) << std::endl;
        return EXIT_FAILURE;
  	} catch (...) {
		LOG << "unexpected error occurred";
		std::cout << "unexpected error!";
        return EXIT_FAILURE;
    }
	return EXIT_SUCCESS;
}
