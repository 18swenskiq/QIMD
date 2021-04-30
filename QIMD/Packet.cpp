#include "Packet.h"
#include <iostream>

Packet::Packet()
{

}

// Acts as deserializing function
Packet::Packet(char rawdata[])
{
	// Read header (hopefully not malformed)
	Begin = static_cast<Instructions::Instruction>(int((rawdata[0]) << 24 |
		(rawdata[1]) << 16 |
		(rawdata[2]) << 8 |
		(rawdata[3])));

	// Read main instruction (hopefully not malformed)
	InputInstruction = static_cast<Instructions::Instruction>(int((rawdata[4]) << 24 |
		(rawdata[5]) << 16 |
		(rawdata[6]) << 8 |
		(rawdata[7])));

	Data = "";
	for (int i = 0; i < 500; i++)
	{
		if (rawdata[i + 8] == '\0') break;
		Data.push_back(rawdata[i + 8]);
	}

	End = static_cast<Instructions::Instruction>(int((rawdata[508]) << 24 |
		(rawdata[509]) << 16 |
		(rawdata[510]) << 8 |
		(rawdata[511])));
}

Packet::Packet(Instructions::Instruction maininstr, std::string data)
{
	InputInstruction = maininstr;
	Data = data;
	Begin = Instructions::Instruction::STARTPACKET;
	End = Instructions::Instruction::ENDPACKET;
}

char* Packet::Serialize()
{
	// We want this to be 512 bytes in total
	char* serializeddata = (char*)calloc(512, sizeof(char));
	if (serializeddata == NULL) return NULL;

	// Write "header" (4 bytes)
	serializeddata[0] = (Instructions::to_underlying(Begin) >> 24) & 0xFF;
	serializeddata[1] = (Instructions::to_underlying(Begin) >> 16) & 0xFF;
	serializeddata[2] = (Instructions::to_underlying(Begin) >> 8) & 0xFF;
	serializeddata[3] = (Instructions::to_underlying(Begin)) & 0xFF;

	// Write main instruction (4 bytes)
	serializeddata[4] = (Instructions::to_underlying(InputInstruction) >> 24) & 0xFF;
	serializeddata[5] = (Instructions::to_underlying(InputInstruction) >> 16) & 0xFF;
	serializeddata[6] = (Instructions::to_underlying(InputInstruction) >> 8) & 0xFF;
	serializeddata[7] = (Instructions::to_underlying(InputInstruction)) & 0xFF;

	// Write data block (500 bytes)
	int strlen = Data.length();
	for (int i = 0; i < strlen; i++)
	{
		serializeddata[i + 8] = Data[i];
	}
	// The rest will be automatically set to zero thanks to calloc. We just have to set the footer now

	// Write footer (4 bytes)
	serializeddata[508] = (Instructions::to_underlying(End) >> 24) & 0xFF;
	serializeddata[509] = (Instructions::to_underlying(End) >> 16) & 0xFF;
	serializeddata[510] = (Instructions::to_underlying(End) >> 8) & 0xFF;
	serializeddata[511] = (Instructions::to_underlying(End)) & 0xFF;

	return serializeddata;
}