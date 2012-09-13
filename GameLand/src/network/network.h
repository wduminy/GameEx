/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#pragma once
#include <winsock2.h>
#include "../systemex/systemex.h"

namespace network {

	class Socket {
		public:
			// creates an IP version 4 TCP socket
			Socket();
			// establish a connection to a server
			void connect(const char * ip_address, const u_short port_number);
			// assign a port to the socket; and opens it for listening
			void bind(const u_short port_number);
			// waits for an incoming connection; returns new socket for that connection
			Socket accept();
			~Socket();
		private:
			Socket(SOCKET socket);
			SOCKET _socket_descriptor;
	};

}
