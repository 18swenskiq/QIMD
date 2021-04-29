#pragma once
#include <thread>
#include <chrono>
#include "winsocket_init.h"
#include "ParseInput.h"
#include "Packet.h"

class qimd_client {
public:
	qimd_client(int port);
	int launch_client();
	bool isCreateSockFailed;

private:
	int connectPort;
	int send_packet(SOCKET ConnectSocket, Packet* packet);
};