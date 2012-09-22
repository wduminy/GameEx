/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */

#pragma once
#include <list>
#include <deque>
#include <forward_list>
#include "gl_objects.h"

namespace game {
	typedef uint32_t time;
	using std::string;


	typedef std::unique_ptr<SDL_Surface> SDL_Surface_u_ptr;

	class ResourceContext {
	public:
		/**
		 *
		 * @param subDirectory must have a '/' at the end
		 */
		ResourceContext(const string& subDirectory = "");
		SDL_Surface_u_ptr load_BMP(const char * filename) const;
		string load_text(const char * filename) const;
	private:
		const string _root_directory;
	};

	class UpdateContext {
		public:
			UpdateContext(unsigned int targetUpdatesPerSecond, unsigned int targetFramesPerSecond);
			bool isUpdate() {return update;}
			bool isDraw() {return draw;}
			unsigned int updateInterval() const {return _update_interval; }
			double updatesPerSecond() const { return 1000.0 / _update_interval; }
			double secondsPerUpdate() const { return 1.0 / updatesPerSecond();}
			void tick();
			SDLKey keyUp() const;
			SDLKey keyDown() const;
			SDL_Event event;
			void logStats() const;
		private:
			const unsigned int _update_interval;
			const unsigned int _draw_interval;
			const Uint32 _first_tick;
			time next_update;
			time next_draw;
			bool update;
			bool draw;
			Uint32 _tick_time;
			unsigned int _draws, _updates;
		public:
			typedef std::unique_ptr<UpdateContext> u_ptr;
	};

	class DrawContext {
		public:
			DrawContext(const bool fullscreen, const int width, const int height);
			~DrawContext();
			Glex& gl() const;
			int width() const {return _width;}
			int height() const {return _height;}
		private:
			Glex::u_ptr instance_p;
			int _width;
			int _height;
	};

	class GameObject {
		public:
			GameObject(int drawOrder = 0) :	_drawOrder(drawOrder) {};
			/** initialise this and initialise children */
			virtual void initialise(const ResourceContext & rctx, const DrawContext& dctx) {};
			/** draw only this, not children */
			virtual void draw(const DrawContext& gc) = 0;
			/** update this and update children */
			virtual void update(const UpdateContext &) {
			}
			virtual void collect(std::deque<GameObject *> &c) {
				c.push_back(this);
			}
			;
			int drawOrder() const {
				return _drawOrder;
			}
			;
			virtual ~GameObject() {
			}
		private:
			const int _drawOrder;
		public:
			typedef std::unique_ptr<GameObject> u_ptr;
	};

	class GameObjectWithParts: public GameObject {
		public:
			GameObjectWithParts(const int drawOrder) :
					GameObject(drawOrder), _parts() {
			}
			;
			// initialises all the parts
			virtual void initialise(const ResourceContext &rctx, const DrawContext& dctx) override {
				for_each(p,_parts) (*p)->initialise(rctx,dctx);
			}
			;
			void update(const UpdateContext &context);
			void add_part(GameObject::u_ptr object) {
				_parts.emplace_after(_parts.before_begin(), std::move(object));
			}
			;
			void collect(std::deque<GameObject*> &c);
		protected:
			std::forward_list<GameObject::u_ptr> _parts;

	};

	class MainObject: public GameObjectWithParts {
		public:
			MainObject(int drawOrder, GLdouble nearest, GLdouble farest)
				: GameObjectWithParts(drawOrder) , _running(true), _nearest(nearest), _farest(farest) {};
            void initialise(const ResourceContext & rctx, const DrawContext& dctx) override;
			void exit() {
				_running = false;
			}
			void update(const UpdateContext& ctx);
			bool isAlive() const {
				return _running;
			}
			;
			void draw(const DrawContext&);
		private:
			bool _running;
			GLdouble _nearest, _farest;

		public:
			typedef std::unique_ptr<MainObject> u_ptr;

	};

	class Game {
		PREVENT_COPY(Game)
		public:
			// the game owns these 'pointers'
			Game(MainObject::u_ptr primaryPart,
				UpdateContext::u_ptr update,
				DrawContext * draw = new DrawContext(false, 640, 480),
				ResourceContext * resource = new ResourceContext(""));
			// the game runs until primary is dead
			void run();
			~Game();
		private:
			MainObject::u_ptr _primary;
			UpdateContext::u_ptr _update;
			DrawContext * draw_ctx;
			ResourceContext * resource_ctx;
	};

}

