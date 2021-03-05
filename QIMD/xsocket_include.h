#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <vector>

#include <WinSock2.h>
#include <WS2tcpip.h>
#define SHUT_RD SD_RECEIVE
#define SHUT_WR	SD_SEND
#define SHUT_RDWR SD_BOTH

#pragma comment(lib, "Ws2_32.lib")

typedef int socklen_t;
static WSAData _wsaData;
static bool _wsaInitDone = false;

namespace net {
	enum class af {
		inet = AF_INET,
		inet6 = AF_INET6,
		unspec = AF_UNSPEC
	};

	enum class sock {
		stream = SOCK_STREAM,
		dgram = SOCK_DGRAM
	};

	enum class shut {
		rd = SHUT_RD,
		wr = SHUT_WR,
		rdwr = SHUT_RDWR
	};
}