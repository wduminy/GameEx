#include "sdl_utils.h"
#include "../systemex/systemex.h"
namespace game {
 	void check(int sdl_result) {
		if (sdl_result)
			throw systemex::runtime_error_ex("SDL call returned %d, error:%s",
					sdl_result, SDL_GetError());
	}

	void check(void * p) {
		if (!p)
			throw systemex::runtime_error_ex("SDL error: %s", SDL_GetError());
	}
}
