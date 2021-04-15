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
	void init();
}
