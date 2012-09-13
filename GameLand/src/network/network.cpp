/**
 * Copyright 2012 Willem Duminy
 * See LICENCE.txt
 */
#include "network.h"

namespace network {

	Socket::Socket() {
		_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
		ENSURE(_socket_descriptor != 0,"could not create socket");
	}


	Socket::Socket(SOCKET socket) {
		_socket_descriptor = socket;
		ENSURE(_socket_descriptor != 0,"invalid socket descriptor");
	}


	void Socket::connect(const char * ip_address, const u_short port_number) {
		// TODO 100 implement connect
		auto address = inet_addr(ip_address);
		if (address == INADDR_NONE)
			throw systemex::runtime_error_ex("could not parse IP address: '%s'", ip_address);
		sockaddr_in target;
		memset(&target,0,sizeof(target));
		target.sin_family = AF_INET;
		target.sin_port = port_number;
		target.sin_addr.S_un.S_addr = address;
		const auto result = ::connect(_socket_descriptor, (sockaddr*)&target, sizeof(target));
		ENSURE(result == 0,"could not connect");
	}

	void Socket::bind(const u_short port_number) {
		sockaddr_in target;
		memset(&target,0,sizeof(target));
		target.sin_family = AF_INET;
		target.sin_port = port_number;
		target.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		auto result = ::bind(_socket_descriptor, (sockaddr*)&target, sizeof(target));
		ENSURE(result == 0,"could not bind");
		result = ::listen(_socket_descriptor,5);
		ENSURE(result == 0,"could not listen");
	}

	Socket Socket::accept() {
		SOCKET result = ::accept(_socket_descriptor,0,0);
		return Socket(result);
	}

	Socket::~Socket() {
		// close the socket
		if (_socket_descriptor != 0)
				::closesocket(_socket_descriptor);
	}
}
