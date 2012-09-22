/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#include "game.h"
#include "../systemex/log.h"
#include <windows.h>

namespace game {
	void check(int sdl_result) {
		if (sdl_result != 0)
			throw systemex::runtime_error_ex("SDL call returned %d, error:%s",
					sdl_result, SDL_GetError());
	}

	void check(void * p) {
		if (p == 0)
			throw systemex::runtime_error_ex("SDL error: %s", SDL_GetError());
	}

    Glex * glexAfterInit(const bool fullscreen, const int w, const int h) {
		check(SDL_Init(SDL_INIT_VIDEO));
		auto version = SDL_Linked_Version();
		LOG << "Using SDL version " << (int) version->major << "." << (int) version->minor << "." << (int) version->patch;
		auto info = SDL_GetVideoInfo();
		auto bpp = info->vfmt->BitsPerPixel;
		int flags = SDL_OPENGL;
		if (fullscreen)
			flags |= SDL_FULLSCREEN;
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		check(SDL_SetVideoMode(w, h, bpp, flags));
		return new Glex();
    }


	DrawContext::DrawContext(const bool fullscreen, const int width, const int height)
			: instance_p(glexAfterInit(fullscreen,width, height))
			, _width(width)
			, _height(height) {}

	Glex& DrawContext::gl() const {
		return *instance_p;
	}

	DrawContext::~DrawContext() {
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}

	void GameObjectWithParts::collect(std::deque<GameObject *> &c) {
		c.push_back(this);
		for_each(it,_parts)
			(*it)->collect(c);
	}

	void GameObjectWithParts::update(const UpdateContext &context) {
		for_each(it,_parts)
			(*it)->update(context);
	}


	/// targetUpdateDelay : milliseconds to update
	UpdateContext::UpdateContext(unsigned int targetUpdatesPerSecond, unsigned int targetFramesPerSecond) :
            event(),
			_update_interval(1000 / targetUpdatesPerSecond),
			_draw_interval(1000 / targetFramesPerSecond),
			_first_tick(SDL_GetTicks()),
            next_update(_first_tick + _update_interval),
            next_draw(_first_tick + _draw_interval),
            update(false),
            draw(false),
            _tick_time(_first_tick),
            _draws(0),
            _updates(0)
             {}

	SDLKey UpdateContext::keyDown() const {
		if (event.type != SDL_KEYDOWN)
			return SDLK_UNKNOWN;
		else
			return event.key.keysym.sym;
	}

	SDLKey UpdateContext::keyUp() const {
		if (event.type != SDL_KEYUP)
			return SDLK_UNKNOWN;
		else
			return event.key.keysym.sym;
	}

	void UpdateContext::tick() {
		_tick_time = SDL_GetTicks();
		if (_tick_time >= next_update) {
			update = true;
			//this could be a problem ... events on SDL is queued and will be
			//processed one at a time of the update loop. so, if the input seems
			//jittery, it could be that the SDL events must be processed in a different
			//way -- maybe an 'input context' would be a good idea
			SDL_PollEvent(&event);
			unsigned int delta = _tick_time - next_update;
			next_update = _tick_time + _update_interval - delta;
			_updates++;
		} else
			update = false;
		if (_tick_time >= next_draw) {
			draw = true;
			next_draw = _tick_time + _draw_interval;
			_draws++;
		} else
			draw = false;
	}

	void UpdateContext::logStats() const {
		auto elapsed =  _tick_time - _first_tick;
		LOG << "Run time (seconds): " << elapsed / 1000.0
		    << "\n\t FPS: " << _draws * 1000.0 / elapsed
		    << "\n\t UPS: " << _updates * 1000.0 / elapsed;
	}

	Game::Game(MainObject::u_ptr primaryPart, UpdateContext::u_ptr update,
			DrawContext * draw, ResourceContext * resource)
				: _primary(std::move(primaryPart))
	            , _update(std::move(update))
	            , draw_ctx(draw)
	            , resource_ctx(resource)
	{}

	Game::~Game() {
		delete resource_ctx;
		delete draw_ctx;
	}

	void Game::run() {
		_primary->initialise(*resource_ctx, *draw_ctx);
		std::deque<GameObject *> objects;
		_primary->collect(objects);
		// to fix draw order, sort the object
		std::sort(objects.begin(), objects.end(),
				[](const GameObject * a, const GameObject * b) {
					return a->drawOrder() < b->drawOrder();});
		_update->tick();
		bool isRunning = true;
		while (isRunning) {
			if (_update->isUpdate()) {
				_primary->update(*_update);
				isRunning = _primary->isAlive();
			}
			if (_update->isDraw()) {
				for (auto it = objects.begin(); it != objects.end(); it++)
					(*it)->draw(*draw_ctx);
				SDL_GL_SwapBuffers();
			}
			_update->tick();
		};
		_update->logStats();
	}

     void MainObject::initialise(const ResourceContext & rctx, const DrawContext& dctx) {
		/* Our shading model--Gouraud (smooth). */
        glShadeModel(GL_SMOOTH);
        /* Culling. */
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glEnable(GL_CULL_FACE);
        glEnable(GL_DEPTH_TEST);
        /* Set the clear color. */
        glClearColor(0.0, 0.0, 0.0, 0);
        /* Setup our viewport. */
        glViewport(0, 0, dctx.width(), dctx.height());
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        float ratio = (float) dctx.width() /  (float) dctx.height();
        glLoadIdentity(); // load identity because we want to 'reset' the perspective
        gluPerspective(45.0, ratio, _nearest, _farest);
        GameObjectWithParts::initialise(rctx,dctx);
	}

	void MainObject::update(const UpdateContext& ctx) {
		GameObjectWithParts::update(ctx);
		switch (ctx.event.type) {
		case SDL_KEYDOWN:
			switch (ctx.event.key.keysym.sym) {
			case SDLK_ESCAPE:
				exit();
				break;
			default:
				break;
			}
			break;
		case SDL_QUIT:
			exit();
			break;
		default:
			break;
		}
	}

	void MainObject::draw(const DrawContext&) {
		/* Clear the color and depth buffers. */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* We don't want to modify the projection matrix. */
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	const std::string RESOURCE_PATH("media/");

	ResourceContext::ResourceContext(const string& subDirectory) : _root_directory(RESOURCE_PATH + subDirectory) {
		if (_root_directory.back() != '/')
			throw systemex::runtime_error_ex("subDirectory (%s) must end with '/'", subDirectory.c_str());
	}

	SDL_Surface_u_ptr ResourceContext::load_BMP(const char * filename) const {
		auto fullname = _root_directory + filename;
		SDL_Surface_u_ptr result(SDL_LoadBMP(fullname.c_str()));
		if (result.get() == 0)
			throw systemex::runtime_error_ex("could not load '%s'", fullname.c_str());
		return result;
	}

	string ResourceContext::load_text(const char * filename) const {
		auto fullname = _root_directory + filename;
		return systemex::string_from_file(fullname.c_str());
	}
}


