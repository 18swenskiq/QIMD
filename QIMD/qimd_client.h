#pragma once
#include <thread>
#include <chrono>
#include "winsocket_init.h"
#include "ParseInput.h"

class qimd_client {
public:
	qimd_client(int port);
	int launch_client();
	bool isCreateSockFailed;

private:
	int connectPort;
};