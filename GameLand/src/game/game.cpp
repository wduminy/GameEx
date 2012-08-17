#include "game.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "../systemex/log.h"
#include <windows.h>

namespace game {

	PROC getGLProc(const char * name) {
		PROC result = wglGetProcAddress(name);
		if (!result)
			systemex::throw_LastError(name);
		return result;
	}



	Glex::Glex() {
		_nearView = 1.0f;
		_farView = 50.0f;
		_height = -1.0f;
		_width = 0;
		glActiveTextureARB = 0;
		glGenFramebuffersEXT = 0;
		glBindFramebufferEXT = 0;
		glFramebufferTexture2DEXT = 0;
		glCheckFramebufferStatusEXT	= 0;
		glCreateShader = 0;
		glCompileShader = 0;
 		glShaderSource = 0;
		glGetShaderiv = 0;
		glDeleteShader = 0;
		glGetGetShaderInfoLog = 0;
		glCreateProgramObjectARB = 0;
		glUseProgramObjectARB = 0;
		glCreateShaderObjectARB = 0;
		glShaderSourceARB = 0;
		glCompileShaderARB = 0;
		glGetObjectParameterivARB = 0;
		glAttachObjectARB = 0;
		glGetInfoLogARB = 0;
		glLinkProgramARB = 0;
		glGetUniformLocationARB = 0;
		glUniform1iARB = 0;
	}

	void Glex::assignFunctionPointers()
	{
		// FBO
		glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)getGLProc("glActiveTextureARB");
		glGenFramebuffersEXT		= (PFNGLGENFRAMEBUFFERSEXTPROC)		getGLProc("glGenFramebuffersEXT");
		glBindFramebufferEXT		= (PFNGLBINDFRAMEBUFFEREXTPROC)		getGLProc("glBindFramebufferEXT");
		glFramebufferTexture2DEXT	= (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)getGLProc("glFramebufferTexture2DEXT");
		glCheckFramebufferStatusEXT	= (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)getGLProc("glCheckFramebufferStatusEXT");

		// Shaders
		glCreateShader = (PFNGLCREATESHADERPROC) getGLProc("glCreateShader");
		glCompileShader = (PFNGLCOMPILESHADERPROC) getGLProc("glCompileShader");
		glShaderSource = (PFNGLSHADERSOURCEPROC) getGLProc("glShaderSource");
		glGetShaderiv = (PFNGLGETSHADERIVPROC) getGLProc("glGetShaderiv");
		glDeleteShader = (PFNGLDELETESHADERPROC) getGLProc("glDeleteShader");
		glGetGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) getGLProc("glGetShaderInfoLog");
		glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)getGLProc("glCreateProgramObjectARB");
		glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)getGLProc("glUseProgramObjectARB");
		glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)getGLProc("glCreateShaderObjectARB");
		glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)getGLProc("glShaderSourceARB");
		glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)getGLProc("glCompileShaderARB");
		glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)getGLProc("glGetObjectParameterivARB");
		glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)getGLProc("glAttachObjectARB");
		glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)getGLProc("glGetInfoLogARB");
		glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)getGLProc("glLinkProgramARB");
		glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)getGLProc("glGetUniformLocationARB");
		glUniform1iARB = (PFNGLUNIFORM1IARBPROC)getGLProc("glUniform1iARB");

	}


	bool Glex::hasExtensions(const char * extension, ...) {
		std::string extensions((const char*) glGetString(GL_EXTENSIONS));
		const char * ext = extension;
		bool result = true;
		va_list args;
		va_start(args, extension);
		do {
			if (extensions.find(ext) == std::string::npos) {
				LOG << "GL Does not support: " << ext;
				result = false;
			} else
				LOG << "GL Supports: " << ext;
			ext = va_arg(args, const char *);
		} while (ext != NULL);va_end(args);
		return result;
	}

	void Glex::updatePerspective() {
		if (_height < 0.0f)
			throw std::runtime_error("height has not been set");
		float ratio = _width / _height;
		glLoadIdentity(); // load identity because we want to 'reset' the perspective
		gluPerspective(60.0, ratio, _nearView, _farView);
	}

	void Glex::throw_error() {
		auto code = glGetError();
		const char * error;
		switch (code) {
		case GL_INVALID_ENUM:
			error = "An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.";
			break;
		case GL_INVALID_VALUE:
			error = "A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.";
			break;
		case GL_INVALID_OPERATION:
			error = "The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.";
			break;
		case GL_STACK_OVERFLOW:
			error = "This command would cause a stack overflow. The offending command is ignored and has no other side effect than to set the error flag.";
			break;
		case GL_STACK_UNDERFLOW:
			error = "This command would cause a stack underflow. The offending command is ignored and has no other side effect than to set the error flag.";
			break;
		case GL_OUT_OF_MEMORY:
			error = "There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
			break;
		case GL_TABLE_TOO_LARGE:
			error = "The specified table exceeds the implementation's maximum supported table size. The offending command is ignored and has no other side effect than to set the error flag.";
			break;
		default:
			error = "An undefined error occurred";
			break;
		}
		throw systemex::runtime_error_ex("%s (%d)",error,code);
	}

	void Glex::initialise(int width, int height) {
		_width = width;
		_height = height;
		std::string version( (const char *) glGetString(GL_VERSION));
		LOG << "Running OpenGL Version " << version;
		if (version[0] < '3')
			throw std::runtime_error("Needs OpenGL version 3.0.0 or later");
		assignFunctionPointers();
		/* Our shading model--Gouraud (smooth). */
		glShadeModel(GL_SMOOTH);
		/* Culling. */
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		/* Set the clear color. */
		glClearColor(0.15, 0.15, 0.3, 0);
		/* Setup our viewport. */
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		updatePerspective();
	}

	void Glex::setViewRange(const float& nearV, const float& farV) {
		if (farV <= nearV)
			throw std::runtime_error("near must be smaller than far");
		_nearView = nearV;
		_farView = farV;
		updatePerspective();
	}

	void check(int sdl_result) {
		if (sdl_result != 0)
			throw systemex::runtime_error_ex("SDL call returned %d, error:%s",
					sdl_result, SDL_GetError());
	}

	void check(void * p) {
		if (p == 0)
			throw systemex::runtime_error_ex("SDL error: %s", SDL_GetError());
	}



	DrawContext::DrawContext(const bool fullscreen, const int width,
			const int height) {
		check(SDL_Init(SDL_INIT_VIDEO));
		auto version = SDL_Linked_Version();
		LOG << "Using SDL version " << (int) version->major << "." << (int) version->minor << "." << (int) version->patch;
		auto info = SDL_GetVideoInfo();
//    This does not work, unfortunately
//		LOG << "Size of video memory is " << info->video_mem / 1024 << " kilobytes";
		auto bpp = info->vfmt->BitsPerPixel;
		int flags = SDL_OPENGL;
		if (fullscreen)
			flags |= SDL_FULLSCREEN;
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		check(SDL_SetVideoMode(width, height, bpp, flags));
		instance.initialise(width, height);
	}

	Glex& DrawContext::gl() {
		return instance;
	}

	DrawContext::~DrawContext() {
		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}

	void GameObjectWithParts::collect(std::deque<GameObject *> &c) {
		c.push_back(this);
		for_each(it,parts)
			(*it)->collect(c);
	}

	void GameObjectWithParts::update(const UpdateContext &context) {
		for_each(it,parts)
			(*it)->update(context);
	}

	GameObjectWithParts::~GameObjectWithParts() {
		systemex::delete_all(parts);
	}

	/// targetUpdateDelay : milliseconds to update
	UpdateContext::UpdateContext(time targetUpdateDelay, time drawUpdateDelay) :
			elapsed_target(targetUpdateDelay), draw_target(drawUpdateDelay) {
		next_update = SDL_GetTicks() + targetUpdateDelay;
		next_draw = 0;
		update = false;
		draw = false;
	}

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
		auto newTime = SDL_GetTicks();
		if (newTime >= next_update) {
			update = true;
			//this could be a problem ... events on SDL is queued and will be
			//processed one at a time of the update loop. so, if the input seems
			//jittery, it could be that the SDL events must be processed in a different
			//way -- maybe an 'input context' would be a good idea
			SDL_PollEvent(&event);
			unsigned int delta = newTime - next_update;
			next_update = newTime + elapsed_target - delta;
		} else
			update = false;
		if (newTime >= next_draw) {
			draw = true;
			next_draw = newTime + draw_target;
		} else
			draw = false;
	}

	Game::Game(MainObject * primaryPart, UpdateContext * update,
			DrawContext * draw, ResourceContext * resource) :
			primary(primaryPart), update_ctx(update), draw_ctx(draw), resource_ctx(
					resource) {
	}

	Game::~Game() {
		delete primary;
		delete resource_ctx;
		delete update_ctx;
		delete draw_ctx;
	}
	void Game::run() {
		primary->initialise(*resource_ctx);
		primary->initialiseDraw(*draw_ctx);
		std::deque<GameObject *> objects;
		primary->collect(objects);
		// to fix draw order, sort the object
		std::sort(objects.begin(), objects.end(),
				[](const GameObject * a, const GameObject * b) {
					return a->drawOrder() < b->drawOrder();});
		update_ctx->tick();
		bool isRunning = true;
		while (isRunning) {
			if (update_ctx->isUpdate()) {
				primary->update(*update_ctx);
				isRunning = primary->isAlive();
			}
			if (update_ctx->isDraw()) {
				for (auto it = objects.begin(); it != objects.end(); it++)
					(*it)->draw(*draw_ctx);
				SDL_GL_SwapBuffers();
			}
			update_ctx->tick();
		};
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

}
