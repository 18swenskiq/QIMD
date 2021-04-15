#include "xsocket_init.hpp"

namespace net {

	void net::init() {
		//no-op on *nix
		if (!_wsaInitDone) {
			WSAStartup(MAKEWORD(2, 2), &_wsaData);
			_wsaInitDone = true;
		}
	}
};