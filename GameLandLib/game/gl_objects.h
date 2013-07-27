/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#pragma once
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "../systemex/systemex.h"
#include "sdl_objects.h"
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
			PFNGLUNIFORM1FPROC glUniform1f;
			PFNGLGENBUFFERSPROC glGenBuffers;
			PFNGLBINDBUFFERPROC glBindBuffer;
			PFNGLBUFFERDATAPROC glBufferData;
			PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
			PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
			PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
			void check_error(const std::string& what = "") const;
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
		void copy_from(const SDL_Surface& surface);
		void copy_from_2d(const SDL_Surface& surface);
		void bind(const int textureIndex);
		void activate();
		GLuint index() const;
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
			void arg(const GLchar * name, const GLfloat value);
			void begin();
			void end();
			bool is_first_time() const { return _first_time; }
			~ShaderProgram();
		private:
			void destroy_shaders();
			GLint compile(const GLchar * source,GLenum type);
			GLint loc(const GLchar * name);
			Glex& _context;
			GLint _vertexShader;
			GLint _fragmentShader;
			GLint _program;
			bool _first_time;
		public:
			typedef std::unique_ptr<ShaderProgram> u_ptr;
	};
}

