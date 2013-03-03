/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#include "log.h"
#include <ostream>

namespace systemex {
	Log s_instance("log.txt");

	Log& Log::instance() {
		return s_instance;
	}

	Log::Log(const std::string& filename) : _file(filename.c_str()), _start(0), _last(0) {
		time(&_start);
		_file.setf(std::ostream::fixed,std::ostream::floatfield);
		LOG << "*** START";
	}

	std::ostream& Log::record() {
		time(&_last);
		_file << " <:" << std::endl << difftime(_last, _start) << ":> ";
		return _file;
	}

	Log::~Log() {
		record() << "*** END";
	}

}
