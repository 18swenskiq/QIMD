#pragma once
#include "xsocket_socket.h"

class qimd_server {
public:
	qimd_server(int port);
	int launch_server();
	bool isCreateSockFailed;

private:
	net::socket* sock;
	net::socket client;
	net::endpoint remoteAddr;
	int connectPort;
};