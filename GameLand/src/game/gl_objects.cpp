// see licence.txt
#include "gl_objects.h"
#include "../systemex/log.h"

namespace game {
using systemex::runtime_error;

PROC getGLProc(const char * name) {
	PROC result = wglGetProcAddress(name);
	if (!result)
		systemex::throw_LastError(name);
	return result;
}

bool Glex::hasExtensions(const char * extension, ...) {
	std::string extensions((const char*) glGetString(GL_EXTENSIONS));
	const char * ext = extension;
	bool result = true;
	va_list args;
	va_start(args, extension);
	do {
		if (extensions.find(ext) == std::string::npos) {
			LOG<< "GL Does not support: " << ext;
			result = false;
		} else
		LOG << "GL Supports: " << ext;
		ext = va_arg(args, const char *);
	} while (ext != NULL);
	va_end(args);
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
		error =
				"An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.";
		break;
	case GL_INVALID_VALUE:
		error =
				"A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.";
		break;
	case GL_INVALID_OPERATION:
		error =
				"The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.";
		break;
	case GL_STACK_OVERFLOW:
		error =
				"This command would cause a stack overflow. The offending command is ignored and has no other side effect than to set the error flag.";
		break;
	case GL_STACK_UNDERFLOW:
		error =
				"This command would cause a stack underflow. The offending command is ignored and has no other side effect than to set the error flag.";
		break;
	case GL_OUT_OF_MEMORY:
		error =
				"There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
		break;
	case GL_TABLE_TOO_LARGE:
		error =
				"The specified table exceeds the implementation's maximum supported table size. The offending command is ignored and has no other side effect than to set the error flag.";
		break;
	default:
		error = "An undefined error occurred";
		break;
	}
	throw systemex::runtime_error_ex("%s (%d)", error, code);
}

Glex::Glex(int width, int height) {
	_nearView = 1.0f;
	_farView = 50.0f;
	_height = -1.0f;
	_width = width;
	_height = height;
	std::string version((const char *) glGetString(GL_VERSION));
	LOG<< "Running OpenGL Version " << version;
	if (version[0] < '3')
		throw std::runtime_error("Needs OpenGL version 3.0.0 or later");
	// FBO
	glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC) getGLProc(
			"glActiveTextureARB");
	glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC) getGLProc(
			"glGenFramebuffersEXT");
	glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC) getGLProc(
			"glBindFramebufferEXT");
	glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC) getGLProc(
			"glFramebufferTexture2DEXT");
	glCheckFramebufferStatusEXT =
			(PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) getGLProc(
					"glCheckFramebufferStatusEXT");

	// Shaders
	glCreateShader = (PFNGLCREATESHADERPROC) getGLProc("glCreateShader");
	glCompileShader = (PFNGLCOMPILESHADERPROC) getGLProc("glCompileShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC) getGLProc("glShaderSource");
	glGetShaderiv = (PFNGLGETSHADERIVPROC) getGLProc("glGetShaderiv");
	glDeleteShader = (PFNGLDELETESHADERPROC) getGLProc("glDeleteShader");
	glGetGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) getGLProc(
			"glGetShaderInfoLog");
	glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC) getGLProc(
			"glCreateProgramObjectARB");
	glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC) getGLProc(
			"glUseProgramObjectARB");
	glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC) getGLProc(
			"glCreateShaderObjectARB");
	glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC) getGLProc(
			"glShaderSourceARB");
	glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC) getGLProc(
			"glCompileShaderARB");
	glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC) getGLProc(
			"glGetObjectParameterivARB");
	glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC) getGLProc(
			"glAttachObjectARB");
	glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC) getGLProc("glGetInfoLogARB");
	glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC) getGLProc("glLinkProgramARB");
	glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC) getGLProc(
			"glGetUniformLocationARB");
	glUniform1iARB = (PFNGLUNIFORM1IARBPROC) getGLProc("glUniform1iARB");

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

Texture::Texture()  {
	_texture = 0;
}

void Texture::bind(SDL_Surface& surface) {
	if ((surface.w & (surface.w - 1)) != 0)
		throw std::runtime_error("surface  width is not a power of 2");
	if ((surface.h & (surface.h - 1)) != 0)
		throw std::runtime_error("surface height is not a power of 2");
	GLenum texture_format;
	auto bpp = surface.format->BytesPerPixel;
	if (bpp == 4) {
		// contains an alpha channel
		if (surface.format->Rmask == 0x000000ff)
			texture_format = GL_RGBA;
		else
			texture_format = GL_BGRA;
	} else if (bpp == 3) {
		// no alpha channel
		if (surface.format->Rmask == 0x000000ff)
			texture_format = GL_RGB;
		else
			texture_format = GL_BGR;
	} else
		throw std::runtime_error("surface is not true colour");
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, bpp, surface.w, surface.h, 0,
			texture_format, GL_UNSIGNED_BYTE, surface.pixels);


}

Texture::~Texture() {
	if (_texture != 0)
		glDeleteTextures(1, &_texture);
}

ShaderProgram::ShaderProgram(Glex& aContext) :
		context(aContext) {
	vertexShader = 0;
	fragmentShader = 0;
}

void ShaderProgram::bind(const string& vertexSource,
		const string& fragmentSource) {
	destroy_shaders();
	vertexShader = compile(vertexSource.c_str(), GL_VERTEX_SHADER);
	fragmentShader = compile(fragmentSource.c_str(), GL_FRAGMENT_SHADER);
}

GLint ShaderProgram::compile(const char * source, GLenum type) {
	auto &g = context;
	GLint shader = g.glCreateShader(type);
	if (!shader)
		g.throw_error();
	const GLchar* src[] = { (source) };
	g.glShaderSource(shader, 1, src, NULL);
	g.glCompileShader(shader);
	GLint result;
	g.glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		// get the error message
		GLint logLength;
		g.glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		char * message = new char[logLength];
		g.glGetGetShaderInfoLog(shader, logLength, &logLength, message);
		std::string error(message);
		delete[] message;
		g.glDeleteShader(shader);
		throw runtime_error(error);
	}
	return shader;
}

void ShaderProgram::destroy_shaders() {
	auto &g = context;
	if (vertexShader)
		g.glDeleteShader(vertexShader);
	vertexShader = 0;
	if (fragmentShader)
		g.glDeleteShader(fragmentShader);
	fragmentShader = 0;
}

ShaderProgram::~ShaderProgram() {
	destroy_shaders();
}

}
