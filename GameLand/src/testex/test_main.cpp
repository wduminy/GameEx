/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#include <tut/tut.hpp>
#include <tut/tut_reporter.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include "../systemex/systemex.h"
using std::exception;
using std::cerr;
using std::endl;
using std::cout;

namespace tut {
	test_runner_singleton runner;
}


void run_tests() {
	WSADATA wsaD;
	ENSURE(WSAStartup(MAKEWORD(2,2),&wsaD) == 0,
			"could not start windows sockets");

	tut::console_reporter reporter(cout);
	tut::runner.get().set_callback(&reporter);
	tut::runner.get().run_tests();
	if (reporter.all_ok())
		cout << "Unit tests passed";
	else
		cout << "Unit tests failed!";
#ifdef FIND_LEAKS
	puts(systemex::memoryLeakReport().c_str());
#endif
	WSACleanup();
}

int main( int , char* [] ) {
	try {
		cout << "RUNNING TESTS";
		run_tests();
	} catch (std::exception &e) {
		cout << "Error --- " << e.what();
	} catch (...) {
		cout << "unexpected error occurred";
	}
	cout.flush();
	return EXIT_SUCCESS;
}


