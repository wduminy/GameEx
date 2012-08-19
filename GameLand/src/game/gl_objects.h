// see licence.txt
#pragma once
#include <SDL/SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include "../systemex/systemex.h"

namespace game {
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
			~ShaderProgram();
		private:
			void destroy_shaders();
			GLint compile(const char * source,GLenum type);
			GLint vertexShader;
			GLint fragmentShader;
			Glex& context;
		public:
			typedef std::unique_ptr<ShaderProgram> u_ptr;
	};
}

