// see licence.txt
#pragma once
#include "../systemex/systemex.h"
#include "game.h"

namespace game {

	class ShaderProgram {
			PREVENT_COPY(ShaderProgram)
		public:
			ShaderProgram(DrawContext& aContext);
			void load(const string& vertexSource, const string& fragmentSource);
			~ShaderProgram();
		private:
			void destroy_shaders();
			GLint compile(const char * source,GLenum type);
			GLint vertexShader;
			GLint fragmentShader;
			DrawContext& context;
	};
}

