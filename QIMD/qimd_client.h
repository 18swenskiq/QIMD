#pragma once
#include <thread>
#include <chrono>
#include "xsocket_socket.h"

class qimd_client {
public:
	qimd_client(int port);
	int launch_client();
	bool isCreateSockFailed;

private:
	net::socket* sock;
	net::socket client;
	net::endpoint remoteAddr;
	int connectPort;
};