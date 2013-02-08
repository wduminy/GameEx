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
		ShaderProgram::u_ptr load_program(Glex& gl, const char * filePrefix) const;
		/**
		 *
		 * @param gl
		 * @param filename
		 * @param textureIndex zero based index where 0 implies
		 * @return
		 */
		Texture::u_ptr load_texture_bmp(Glex& gl, const char * filename, const int textureIndex) const;
	private:
		const string _root_directory;
	public:
		typedef std::unique_ptr<ResourceContext> u_ptr;
	};

	class UpdateContext {
		public:
			UpdateContext(unsigned int targetUpdatesPerSecond, unsigned int targetFramesPerSecond);
			bool is_update_step() {return update;}
			bool is_draw_step() {return draw;}
			unsigned int update_interval() const {return _update_interval; }
			double updates_per_second() const { return 1000.0 / _update_interval; }
			double seconds_per_update() const { return 1.0 / updates_per_second();}
			void tick();
			SDLKey key_up() const;
			SDLKey key_down() const;
			SDL_Event event() const { return _event;}
			void log_statistics() const;
		private:
			SDL_Event _event;
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
		public:
			typedef std::unique_ptr<DrawContext> u_ptr;
	};

	class GameObject {
		public:
			GameObject(int drawOrder = 0) :	_draw_order(drawOrder) {};
			/** initialise this and initialise children */
			virtual void initialise(const ResourceContext & rctx, const DrawContext& dctx) {};
			/** draw only this, not children */
			virtual void draw(const DrawContext& gc) = 0;
			/** update this and update children */
			virtual void update(const UpdateContext &) {}
			virtual void collect(std::deque<GameObject *> &c) {c.push_back(this);}
			int draw_order() const {return _draw_order;}
			virtual ~GameObject() {}
		private:
			const int _draw_order;
		public:
			typedef std::unique_ptr<GameObject> u_ptr;
	};

	class GameObjectWithParts: public GameObject {
		public:
			GameObjectWithParts(const int drawOrder) :
					GameObject(drawOrder), _parts() {}
			// initialises all the parts
			virtual void initialise(const ResourceContext &rctx, const DrawContext& dctx) override {
				for (auto p = _parts.begin(); p != _parts.end(); p++) (*p)->initialise(rctx,dctx);
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
			void exit() { _running = false;}
			void update(const UpdateContext& ctx) override;
			bool is_alive() const {return _running;}
			void draw(const DrawContext&) override;
		private:
			bool _running;
			GLdouble _nearest, _farest;
		public:
			typedef std::unique_ptr<MainObject> u_ptr;
	};

	class Game {
		public:
			// the game owns these 'pointers'
			Game(MainObject::u_ptr primaryPart,
				UpdateContext::u_ptr update,
				DrawContext::u_ptr draw,
				ResourceContext::u_ptr resource);
			// the game runs until primary is dead
			void run();
		private:
			MainObject::u_ptr _primary;
			UpdateContext::u_ptr _update;
			DrawContext::u_ptr _draw;
			ResourceContext::u_ptr _resource;
	};

}

