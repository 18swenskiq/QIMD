#pragma once
#include "winsocket_init.h"

class qimd_server {
public:
	qimd_server(int port);
	int launch_server();
	bool isCreateSockFailed;

private:
	int connectPort;
};