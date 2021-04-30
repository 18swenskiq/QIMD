#pragma once
#include "winsocket_init.h"
#include "Packet.h"
#include "QIMDTable.h"

class qimd_server {
public:
	qimd_server(int port);
	int launch_server();
	bool isCreateSockFailed;
	std::string returnvalue;

private:
	std::vector<QIMDTable*> tables;
	QIMDTable* curtable;
	int numtables;
	int parse_packet(Packet* packet);
	int switch_table(std::string datapayload);
	int connectPort;
};