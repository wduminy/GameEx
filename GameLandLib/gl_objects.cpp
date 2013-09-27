/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#include "gl_objects.h"
#include "log.h"

namespace game {
using systemex::runtime_error;
using systemex::runtime_error_ex;
bool gl_initialised = false;

void init_opengl() {
    if (gl_initialised)
        return;

    const char * glv = (const char *) glGetString(GL_VERSION);
    if (!glv)
        throw runtime_error("opengl context not setup");
    std::string version(glv);
    gl_initialised = true;
	LOG<< "Running OpenGL Version " << version;
	const char required[] = "3.1.0\0";
	bool versionOk = true;
	for (size_t i=0;i<sizeof(required)-1;i++) {
		versionOk = version[i] >= required[i];
		if (!versionOk)
			break;
	}
	if (!versionOk)
		throw systemex::runtime_error_ex("This program needs OpenGL version %s or later.\nTry updating your display adapter driver.\nNote that old graphic adaptors do not support this version.",required);
}

PROC getGLProc(const char * name) {
    init_opengl();
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

void Glex::check_error(const std::string& what) const {
	auto code = glGetError();
	if (code == NO_ERROR)
        return;
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
				"GL_INVALID_OPERATION. The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.";
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
	throw systemex::runtime_error_ex("'%s': %s (%d)", what.c_str(), error, code);
}

Glex::Glex()
	: glGenFramebuffersEXT((PFNGLGENFRAMEBUFFERSEXTPROC) getGLProc("glGenFramebuffersEXT"))
	, glBindFramebufferEXT ((PFNGLBINDFRAMEBUFFEREXTPROC) getGLProc("glBindFramebufferEXT"))
	, glFramebufferTexture2DEXT ((PFNGLFRAMEBUFFERTEXTURE2DEXTPROC) getGLProc("glFramebufferTexture2DEXT"))
	, glCheckFramebufferStatusEXT ((PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) getGLProc("glCheckFramebufferStatusEXT"))
    , glCreateShader ((PFNGLCREATESHADERPROC) getGLProc("glCreateShader"))
	, glCompileShader ((PFNGLCOMPILESHADERPROC) getGLProc("glCompileShader"))
	, glGetShaderiv ((PFNGLGETSHADERIVPROC) getGLProc("glGetShaderiv"))
	, glDeleteShader ((PFNGLDELETESHADERPROC) getGLProc("glDeleteShader"))
	, glGetGetShaderInfoLog ((PFNGLGETSHADERINFOLOGPROC) getGLProc("glGetShaderInfoLog"))
	, glShaderSource ((PFNGLSHADERSOURCEPROC) getGLProc("glShaderSource"))
	, glCreateProgram ((PFNGLCREATEPROGRAMPROC) getGLProc("glCreateProgram"))
	, glUseProgram ((PFNGLUSEPROGRAMPROC) getGLProc("glUseProgram"))
	, glAttachShader ((PFNGLATTACHSHADERPROC) getGLProc("glAttachShader"))
	, glLinkProgram  ((PFNGLLINKPROGRAMPROC) getGLProc("glLinkProgram"))
	, glGetProgramiv  ((PFNGLGETPROGRAMIVPROC) getGLProc("glGetProgramiv"))
	, glDeleteProgram  ((PFNGLDELETEPROGRAMPROC) getGLProc("glDeleteProgram"))
	, glActiveTexture ((PFNGLACTIVETEXTUREPROC) getGLProc("glActiveTexture"))
	, glGetUniformLocation	((PFNGLGETUNIFORMLOCATIONPROC) getGLProc("glGetUniformLocation"))
	, glUniform1i ((PFNGLUNIFORM1IPROC) getGLProc("glUniform1i"))
	, glUniform1f ((PFNGLUNIFORM1FPROC) getGLProc("glUniform1f"))
	, glGenBuffers ((PFNGLGENBUFFERSPROC) getGLProc("glGenBuffers"))
	, glBindBuffer ((PFNGLBINDBUFFERPROC) getGLProc("glBindBuffer"))
	, glBufferData ((PFNGLBUFFERDATAPROC) getGLProc("glBufferData"))
	, glEnableVertexAttribArray ((PFNGLENABLEVERTEXATTRIBARRAYPROC) getGLProc("glEnableVertexAttribArray"))
 	, glDisableVertexAttribArray ((PFNGLDISABLEVERTEXATTRIBARRAYPROC) getGLProc("glDisableVertexAttribArray"))
 	, glVertexAttribPointer ((PFNGLVERTEXATTRIBPOINTERPROC) getGLProc("glVertexAttribPointer"))
	, glBindVertexArray ((GLF_U) getGLProc("glBindVertexArray"))
	, glGenVertexArrays ((GLF_S_PU) getGLProc("glGenVertexArrays"))
	, glBindAttribLocation ((PFNGLBINDATTRIBLOCATIONPROC) getGLProc("glBindAttribLocation")) 
	, glGetAttribLocation ((PFNGLGETATTRIBLOCATIONPROC) getGLProc("glGetAttribLocation")) {}

Texture::Texture(Glex * context) : _context(context), _texture(0), _texture_index(-1)  {
	glGenTextures(1, &_texture);
}

void Texture::copy_from(const SDL_Surface& surface) {
	if ((surface.w & (surface.w - 1)) != 0)
		throw runtime_error_ex("surface  width is not a power of 2 it is %d", surface.w);
	if ((surface.h & (surface.h - 1)) != 0)
		throw std::runtime_error("surface height is not a power of 2");
	copy_from_2d(surface);
}

void Texture::copy_from_2d(const SDL_Surface& surface) {
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
		throw runtime_error_ex("surface has invalid bytes per pixel: %d",bpp);
	activate();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, bpp, surface.w, surface.h, 0,
			texture_format, GL_UNSIGNED_BYTE, surface.pixels);
    _context->check_error();
}


void Texture::bind(const int textureIndex) {
    _texture_index = textureIndex;
    activate();
}

void Texture::activate() {
	if (_texture_index < 0)
		throw runtime_error("texture has not been bound");
    _context->glActiveTexture(GL_TEXTURE0 + _texture_index);
    glBindTexture(GL_TEXTURE_2D,_texture);
    _context->check_error("bind texture");
}
GLuint Texture::index() const {
	if (_texture_index == -1)
		throw runtime_error("cannot use index because texture has not been activated yet");
	return _texture_index;
}

Texture::~Texture() {
	if (_texture != 0)
		glDeleteTextures(1, &_texture);
}

ShaderProgram::ShaderProgram()
    : _context(0)
	, _vertexShader(0)
	, _fragmentShader(0)
	, _program(0)
    , _first_time(true) {}

#define CHECK_CONTEXT if (!_context) throw runtime_error("context not intialized")
void ShaderProgram::initialise(Glex * gl,const string& vertexSource,const string& fragmentSource) {
	_context = gl;
	destroy_shaders();
	_vertexShader = compile(vertexSource.c_str(), GL_VERTEX_SHADER);
	_fragmentShader = compile(fragmentSource.c_str(), GL_FRAGMENT_SHADER);
    _program = _context->glCreateProgram();
    _context->glAttachShader(_program,_vertexShader);
    _context->glAttachShader(_program,_fragmentShader);
    _context->glLinkProgram(_program);
    GLint result;
    _context->glGetProgramiv(_program,GL_LINK_STATUS, &result);
    if (result == GL_FALSE)
        throw runtime_error("link failed");
}

GLint ShaderProgram::loc(const GLchar * name) {
    const auto result = _context->glGetUniformLocation(_program,name);
    if (result == -1)
        throw systemex::runtime_error_ex("Could not locate uniform variable '%s'",name);
    return result;
}

void ShaderProgram::arg(const GLchar * name, const GLuint value) {
	CHECK_CONTEXT;
    _context->glUniform1i(loc(name),value);
    _context->check_error(string("setting uniform variable: ") + name);
}

void ShaderProgram::arg(const GLchar * name, const GLfloat value) {
	CHECK_CONTEXT;
    _context->glUniform1f(loc(name),value);
    _context->check_error(string("setting uniform variable: ") + name);
}

void ShaderProgram::begin() {
	CHECK_CONTEXT;
    _context->glUseProgram(_program);
}

void ShaderProgram::end() {
    _context->glUseProgram(0);
    _first_time = false;
}

GLint ShaderProgram::compile(const char * source, GLenum type) {
	CHECK_CONTEXT;
	auto &g = *_context;
	GLint shader = g.glCreateShader(type);
	if (!shader)
		g.check_error();
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
		error = error + "***\n" + source + "\n****";
		throw runtime_error(error);
	}
	return shader;
}

void ShaderProgram::destroy_shaders() {
	if (!_context)
		return;
	if (_program)
        _context->glDeleteProgram(_program);
	if (_vertexShader)
		_context->glDeleteShader(_vertexShader);
	_vertexShader = 0;
	if (_fragmentShader)
		_context->glDeleteShader(_fragmentShader);
	_fragmentShader = 0;
}

void ShaderProgram::bind_attribute(GLuint location, const GLchar * name) {
	CHECK_CONTEXT;
	_context->glBindAttribLocation(_program,location,name);
	_context->check_error("bind attribute");
}

GLuint ShaderProgram::attribute_location(const GLchar * name) {
	CHECK_CONTEXT;
	const auto result = _context->glGetAttribLocation(_program, name);
	_context->check_error();
	if (result == -1)
		throw runtime_error_ex("could not find name '%s'" , name);
	return result;
}

ShaderProgram::~ShaderProgram() {
	destroy_shaders();
}

}
