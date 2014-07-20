// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0
#pragma once 
#include <string>
#include <fstream> 
#include <time.h>
 
#define LOG ::codespear::Log::instance().record()
#define TRACE (::codespear::Log::instance().record() << __FILE__ << "(" << __LINE__ << "):")

namespace codespear {
 
	class Log {
		public:
			static Log& instance();
			Log(const std::string& fileName);
			std::ostream& record();
			std::ostream& file() {return _file;}
			virtual ~Log();
		private:
			std::ofstream _file;
			time_t _start;
			time_t _last;
	};

}
