/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#pragma once
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "../systemex/systemex.h"
typedef const GLfloat* GLfloat_ptr;

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
			PFNGLACTIVETEXTUREPROC glActiveTexture;
			PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
			PFNGLUNIFORM1IPROC glUniform1i;
			void check_error();
			void setViewRange(const float&, const float&);
		private:
			// NB: terminate arg list with NULL
			bool hasExtensions(const char * extension, ...);
		public:
			Glex();
			typedef std::unique_ptr<Glex> u_ptr;
	};

	class Texture {
	public:
		Texture(Glex &context);
		void copy_from(SDL_Surface& surface);
		void activate(const int textureIndex);
		int index() const;
		~Texture();
	private:
        Glex& _context;
		GLuint _texture;
		int _texture_index;
    public:
        typedef std::unique_ptr<Texture> u_ptr;
	};

	class ShaderProgram {
			PREVENT_COPY(ShaderProgram)
		public:
			ShaderProgram(Glex& aContext);
			void bind(const string& vertexSource, const string& fragmentSource);
			void arg(const GLchar * name, const GLuint value);
			void begin();
			void end();
			~ShaderProgram();
		private:
			void destroy_shaders();
			GLint compile(const GLchar * source,GLenum type);
			GLint loc(const GLchar * name);
			Glex& _context;
			GLint _vertexShader;
			GLint _fragmentShader;
			GLint _program;
		public:
			typedef std::unique_ptr<ShaderProgram> u_ptr;
	};
}

