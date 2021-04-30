#pragma once
#include "winsocket_init.h"
#include "Packet.h"
#include "QIMDTable.h"

class qimd_server {
public:
	qimd_server(int port);
	int launch_server();
	bool isCreateSockFailed;

private:
	std::vector<QIMDTable*> tables;
	int numtables;
	int parse_packet(Packet* packet);
	int connectPort;
};