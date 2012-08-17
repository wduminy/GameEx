#pragma once
#include <stdexcept>
#include <string>
#include <stdarg.h>
#include <algorithm>
#include <functional>
#include <list>
#define PREVENT_COPY(X) private: X(const X &source); X & operator=(const X&);
#define ENSURE(P,M) if (!(P)) throw std::runtime_error(M)
#define for_all_m(C,F,A) systemex::for_all(C,std::bind2nd(std::mem_fun(&F),A))


// finding memory leaks
#ifdef FIND_LEAKS
	#define malloc(size) systemex::track_malloc(size,  __FILE__, __LINE__)
	#define free(ptr) systemex::track_free(ptr, __FILE__, __LINE__)
namespace systemex {
	void *track_malloc(size_t size,  const char *file, int line);
	void  track_free(void *ptr, const char *file, int line);
	std::string memoryLeakReport();
}
#else
	#include <stdlib.h>
#endif
/**
 * Extends standard system facilities
 */
namespace systemex {
	using std::runtime_error;
	using std::exception;
	using std::string;

	// caller must delete[] result
	char * cstring_copy(const char *);

	string string_from_format(const char *szFormat, ...);
	// creates a directory if it is not found
	void create_dir(const string& path);
	string string_from_file(const char * fileName);
	void throw_LastError(const string& message);

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

	class file_not_found: public runtime_error_ex {
		public:
			file_not_found(const char *pszFileName);
	};

	class parse_error: public runtime_error_ex {
		public:
			parse_error(const char *pszExpected, const char *pszFound = "EOF");
	};

	class not_implemented: public runtime_error_ex {
		public:
			not_implemented(const char *pszWhat);
	};

	/**
	 * Answer cannot be copied; but does support r-value moves
	 */
	class Answer {
			PREVENT_COPY(Answer)
		public:
			// constructs a true answer
			explicit Answer();
			Answer(bool value, const char * pFormat, ...);
			// move constructor
			Answer(Answer&& source);
			const char * what() const;
			operator bool() const {return _value; }
			// move assignment
			Answer& operator=(Answer&& rhs);
			~Answer();
		private:
			// move data from rhs to this
			void moveData(Answer && rhs);
			char *  _what;
			bool _value;
	};

	template <class T, class F> inline
	void for_all(T coll, F fun) {std::for_each(coll.begin(), coll.end(), fun);};

	template <class E> inline void deleteF(E e) {delete e;};

	template <class T> inline void delete_all(std::list<T> coll) {for_all(coll,deleteF<T>);};

}
#define for_each(I,C) for(auto I = C.begin(); I != C.end(); ++I)
