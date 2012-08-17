// see licence.txt
#include "shader.h"
namespace game {
	using systemex::runtime_error;

	ShaderProgram::ShaderProgram(DrawContext& aContext) : context(aContext) {
		vertexShader = 0;
		fragmentShader = 0;
	}

	void ShaderProgram::load(const string& vertexSource, const string& fragmentSource) {
		destroy_shaders();
		vertexShader = compile(vertexSource.c_str(), GL_VERTEX_SHADER);
		fragmentShader = compile(fragmentSource.c_str(), GL_FRAGMENT_SHADER);
	}

	GLint ShaderProgram::compile(const char * source, GLenum type) {
		auto &g = context.gl();
		GLint shader = g.glCreateShader(type);
		if (!shader)
			g.throw_error();
		const GLchar* src[] = {(source)};
		g.glShaderSource(shader,1, src, NULL);
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
		auto &g = context.gl();
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
