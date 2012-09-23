/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#include "../testex/test_util.h"
#include "network.h"

#define TEST template<> template<> void test_group<networkdata>::object::test
namespace tut {
	using namespace network;

	struct networkdata { virtual ~networkdata(){};};
	test_group<networkdata> networkTests("020 Network Subsystem");

	TEST<1>() {
		set_test_name("client connect fails because IP address is invalid format");
		const auto bad_ip = "127_0_0_1";
		try {
			Socket soc;
			soc.connect(bad_ip,9999);
			fail("exception not thrown");
		} catch (std::runtime_error &ex) {
			ensure_contains(ex,bad_ip);
		}
	}

	TEST<2>() {
		set_test_name("client connect fails because there is no server");
		try {
			Socket soc;
			soc.connect("127.0.0.1",9999);
			fail("exception not thrown");
		} catch (std::runtime_error &ex) {
			ensure_contains(ex,"not connect");
		}
	}

	TEST<3>() {
		set_test_name("server bind to bound port fails");
		Socket soc1;
		soc1.bind(12345);
		try {
			Socket soc2;
			soc2.bind(12345);
			fail("exception not thrown");
		} catch (std::runtime_error &ex) {
			ensure_contains(ex,"not bind");
		}
	}

	TEST<4>() {
		set_test_name("server bind again fails");
		try {
			Socket soc1;
			soc1.bind(12345);
			soc1.bind(12345);
			fail("exception not thrown");
		} catch (std::runtime_error &ex) {
			ensure_contains(ex,"not bind");
		}
	}


	TEST<5>() {
		set_test_name("server second bind succeeds");
		{
			Socket soc1;
			soc1.bind(12345);
		}
		Socket soc2;
		soc2.bind(12345);
	}

	class callable
	{
		public:
	    void operator()() {
	    	using namespace std;
	    	cout << "hello world" << endl;
	    }
	};

  TEST<6>() {
  	set_test_name("accept never dies");
		Socket soc;
		soc.bind(12345);
//		auto r = callable();
		// these lines crash on mingw gcc
//		Sleep(100);
//		boost::thread acceptor(r);
//		acceptor.join();
  }



}
