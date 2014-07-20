// Copyright (c) 2014 Willem Duminy
// License: GNU General Public License, version 3 (GPL-3.0)
// License page: http://opensource.org/licenses/GPL-3.0

#pragma once 
#include <stdexcept>
#include <string> 
#include <sstream>

#define ENSURE(P,M) if (!(P)) throw codespear::runtime_error_ex("%s %s %d", M, __FILE__,__LINE__)
#define check_not_null(P) ENSURE(P!=0,"value cannot be null")
#define check_that(P) ENSURE(P,"predicate is false"); 
#ifdef NDEBUG
#define ASSERT(P) /*SKIP*/
#else
#define ASSERT(P) if (!(P)) throw codespear::runtime_error_ex("assert fails: %s %s %d", #P, __FILE__,__LINE__)
#endif
// Example: THROW_STREAM("hello " << 12 * 23)
#define THROW_STREAM(S) {std::ostringstream ss; ss << S; throw runtime_error(ss.str());}

namespace codespear {
	using std::runtime_error;
	using std::exception;
	using std::string;

	// caller must delete[] result
	char * cstring_copy(const char *);
	// throws exception if the file does not exist
	void ensure_file_exists(const char * filename);
	string string_from_format(const char *szFormat, ...);
	// creates a directory if it is not found
	void create_dir(const string& path);
	string string_from_file(const char * fileName);

	/**
	 * \brief An exception that supports formatted input
	 */
	class runtime_error_ex: public runtime_error {
		public:
			runtime_error_ex(const char *pFormat, ...);
			virtual const char *what() const throw ();
			~runtime_error_ex() throw ();
		private:
			std::string m_message;
		protected:
			runtime_error_ex();
	};


}


