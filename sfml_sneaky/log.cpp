// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0
#include "log.h"
#include <ostream>

namespace codespear {
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
		_file << " <:" << std::endl << _last - _start << ":> ";
		return _file;
	}

	Log::~Log() {
		record() << "*** END";
	}

}
