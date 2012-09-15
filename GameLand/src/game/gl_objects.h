/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "../systemex/systemex.h"

namespace game {
	using std::string;

	// openGL extensions and helpers
	class Glex {
		PREVENT_COPY(Glex)
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
			PFNGLCREATEPROGRAMPROC glCreateProgram;
			PFNGLUSEPROGRAMPROC glUseProgram;
			PFNGLATTACHSHADERPROC glAttachShader;
			PFNGLLINKPROGRAMPROC glLinkProgram;
			PFNGLGETPROGRAMIVPROC glGetProgramiv;
			PFNGLDELETEPROGRAMPROC glDeleteProgram;
			void throw_error();
			void setViewRange(const float&, const float&);
		private:
			// NB: terminate arg list with NULL
			bool hasExtensions(const char * extension, ...);
			void updatePerspective();
			GLdouble _nearView;
			GLdouble _farView;
			GLfloat _width;
			GLfloat _height;
		public:
			Glex(int width, int height);
			typedef std::unique_ptr<Glex> u_ptr;
	};

	class Texture {
		PREVENT_COPY(Texture)
	public:
		Texture();
		void bind(SDL_Surface& surface);
		~Texture();
	private:
		GLuint _texture;
	};

	class ShaderProgram {
			PREVENT_COPY(ShaderProgram)
		public:
			ShaderProgram(Glex& aContext);
			void bind(const string& vertexSource, const string& fragmentSource);
			void begin();
			void end();
			~ShaderProgram();
		private:
			void destroy_shaders();
			GLint compile(const char * source,GLenum type);
			Glex& _context;
			GLint _vertexShader;
			GLint _fragmentShader;
			GLint _program;
		public:
			typedef std::unique_ptr<ShaderProgram> u_ptr;
	};
}

