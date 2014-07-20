// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0
#include <string.h>
#include <stdio.h>
#include "exception.h"
#include <fstream>
#include <stdarg.h>
#include <memory>

using namespace std;

namespace codespear {


	char * cstring_copy(const char *str) {
		size_t len = strlen(str);
		/* 1 for the null terminator */
		char *result = new char[len+1];
		ENSURE(result != 0, "could not allocate memory");
		memcpy(result, str, len + 1);
		return result;
	}

	string string_from_format(const char *szFormat, ...) {
		char buffer[1024];
		buffer[sizeof(buffer) - 1] = 0;
		va_list args;
		va_start(args, szFormat);
		vsnprintf(buffer, sizeof(buffer) - 1, szFormat, args);
		va_end(args);
		return string(buffer);
	}

	runtime_error_ex::runtime_error_ex(const char *szFormat, ...) :
			runtime_error("Extended"), m_message() {
		char buffer[1024];
		buffer[sizeof(buffer) - 1] = 0;
		va_list args;
		va_start(args, szFormat);
		vsnprintf(buffer, sizeof(buffer) - 1, szFormat, args);
		va_end(args);
		m_message = std::string(buffer);
	}

	runtime_error_ex::runtime_error_ex(void) :
			runtime_error("Extended"), m_message() {
	}

	runtime_error_ex::~runtime_error_ex() throw () {
	}

	const char *runtime_error_ex::what() const throw () {
		return m_message.c_str();
	}


}


