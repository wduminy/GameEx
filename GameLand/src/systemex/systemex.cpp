#include <string.h>
#include <stdio.h>
#include "systemex.h"
#include <fstream>
#include <windows.h>
#include <dirent.h>

using namespace std;

namespace systemex {

	void create_dir(const string& path) {
			if (mkdir(path.c_str()) != 0) {
				if (errno != EEXIST)
					throw runtime_error_ex("could not create directory:'%s'",path.c_str());
			}
	}

	void throw_LastError(const string& message) {
	    // Retrieve the system error message for the last-error code
	    void * pszMessage;
	    auto dw = GetLastError();
	    FormatMessage(
	          FORMAT_MESSAGE_ALLOCATE_BUFFER |
	          FORMAT_MESSAGE_FROM_SYSTEM |
	          FORMAT_MESSAGE_IGNORE_INSERTS,
	          NULL,
	          dw,
	          MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
	          (LPTSTR) &pszMessage,
	          0, NULL );
	    const string msg(string_from_format("%s: %s",message.c_str(), pszMessage));
	    LocalFree(pszMessage);
	    throw runtime_error(msg);
	}


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

	string string_from_file(const char * fileName) {
		ifstream file;
		file.open(fileName);
		if (!file)
			throw file_not_found(fileName);
		// determine file size
		file.seekg(0,ios::end);
		auto length = file.tellg();
		if (length == 0)
			throw runtime_error_ex("The file ('%s') is empty", fileName);
		// read the whole file the result
		file.seekg(0,ios::beg);
		const size_t bufSize = (unsigned int) length + 1;
		string result;
		result.reserve(bufSize);
		char c;
		file.get(c);
		do {
			result += c;
			file.get(c);
		} while (file.good());
		return result;
	}


	static const char *PARSE_ERROR = "Looking for '%s' but found '%s'";
	file_not_found::file_not_found(const char *str) :
			runtime_error_ex("File could not be opened:%s", str) {

	}

	not_implemented::not_implemented(const char *str) :
			runtime_error_ex("Not implemented:%s", str) {

	}

	parse_error::parse_error(const char *strExpected, const char *strFound) :
			runtime_error_ex(PARSE_ERROR, strExpected, strFound) {
	}

	runtime_error_ex::runtime_error_ex(const char *szFormat, ...) :
			runtime_error("Extended") {
		char buffer[1024];
		buffer[sizeof(buffer) - 1] = 0;
		va_list args;
		va_start(args, szFormat);
		vsnprintf(buffer, sizeof(buffer) - 1, szFormat, args);
		va_end(args);
		m_message = std::string(buffer);
	}

	runtime_error_ex::runtime_error_ex(void) :
			runtime_error("Extended") {
	}

	runtime_error_ex::~runtime_error_ex() throw () {
	}

	const char *runtime_error_ex::what() const throw () {
		return m_message.c_str();
	}

	Answer::Answer() {
		_what = 0;
		_value = true;
	}

	Answer::Answer(bool value, const char * format, ...) {
		char buffer[1024];
		buffer[sizeof(buffer) - 1] = 0;
		va_list args;
		va_start(args, format);
		vsnprintf(buffer, sizeof(buffer) - 1, format, args);
		va_end(args);
		_what = cstring_copy(buffer);
		_value = value;
	}

	Answer::Answer(Answer&& rhs) {
		moveData(static_cast<Answer&&>(rhs));
	}

	Answer::~Answer() {
		if (_what != 0)
			delete[] _what;
	}

	Answer& Answer::operator=(Answer &&rhs) {
		moveData(static_cast<Answer&&>(rhs));
		return *this;
	}

	const char * Answer::what() const {
		if (_what == 0)
			return "No explanation";
		return _what;
	}

	void Answer::moveData(Answer&& rhs) {
		_what = rhs._what;
		_value = rhs._value;
		rhs._what = 0;
	}

}
