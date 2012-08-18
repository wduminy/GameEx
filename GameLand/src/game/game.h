#pragma once
#include <list>
#include <deque>
#include "../systemex/systemex.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <forward_list>
#include <memory>

namespace game {
	typedef uint32_t time;
	using std::string;

	// openGL extensions and helpers
	class Glex {
		PREVENT_COPY(Glex)
		friend class DrawContext;
		public:
			// FrameBuffer (FBO) gen, bin and texturebind
			PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT;
			PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT;
			PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT;
			PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT;
			// Shader functions
			PFNGLCREATESHADERPROC glCreateShader;
			PFNGLCOMPILESHADERPROC glCompileShader;
			PFNGLGETSHADERIVPROC glGetShaderiv;
			PFNGLDELETESHADERPROC glDeleteShader;
			PFNGLGETSHADERINFOLOGPROC glGetGetShaderInfoLog;
			PFNGLSHADERSOURCEPROC glShaderSource;
			PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
			PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
			PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
			PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
			PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
			PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
			PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
			PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
			PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
			PFNGLUNIFORM1IARBPROC glUniform1iARB;
			PFNGLACTIVETEXTUREARBPROC glActiveTextureARB;
			PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
			void throw_error();
			void setViewRange(const float&, const float&);
		private:
			Glex(int width, int height);
			// NB: terminate arg list with NULL
			bool hasExtensions(const char * extension, ...);
			void updatePerspective();
			GLdouble _nearView;
			GLdouble _farView;
			GLfloat _width;
			GLfloat _height;
		public:
			typedef std::unique_ptr<Glex> u_ptr;
	};

	class ResourceContext {

	};

	class UpdateContext {
		PREVENT_COPY(UpdateContext)
		public:
			UpdateContext(time targetUpdateDelay, time drawUpdateDelay = 30);
			unsigned int get_elapsed_milliseconds() const {
				return elapsed_target;
			}
			;
			bool isUpdate() {
				return update;
			}
			;
			bool isDraw() {
				return draw;
			}
			;
			void tick();
			SDLKey keyUp() const;
			SDLKey keyDown() const;
			SDL_Event event;
		protected:

		private:
			const unsigned int elapsed_target;
			const unsigned int draw_target;
			time next_update;
			time next_draw;
			bool update;
			bool draw;
	};

	class DrawContext {
		public:
			DrawContext(const bool fullscreen, const int width, const int height);
			~DrawContext();
			Glex& gl();
		private:
			Glex::u_ptr instance_p;
	};

	class GameObject {
		public:
			GameObject(int drawOrder = 0) :	_drawOrder(drawOrder) {}
			;
			virtual void initialise(const ResourceContext &) {}
			;
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
					GameObject(drawOrder) {
			}
			;
			// initialises all the parts
			virtual void initialise(const ResourceContext &context) {
				for_each(p,_parts) (*p)->initialise(context);
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
			MainObject(int drawOrder) :
					GameObjectWithParts(drawOrder) {
				running = true;
			}
			;
			void exit() {
				running = false;
			}
			void update(const UpdateContext& ctx);
			bool isAlive() const {
				return running;
			}
			;
			void draw(const DrawContext&);
			// initialise the draw context. called once before the game starts
			virtual void initialiseDraw(DrawContext &) {}
		private:
			bool running;
	};

	class Game {
		PREVENT_COPY(Game)
		public:
			// the game owns these 'pointers'
			Game(MainObject * primaryPart,
					UpdateContext * update = new UpdateContext(100), DrawContext * draw =
							new DrawContext(false, 640, 480), ResourceContext * resource =
							new ResourceContext());
			// the game runs until primary is dead
			void run();
			~Game();
		private:
			MainObject * primary;
			UpdateContext * update_ctx;
			DrawContext * draw_ctx;
			ResourceContext * resource_ctx;
	};

}

