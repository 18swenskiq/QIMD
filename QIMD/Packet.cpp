#include "Packet.h"

Packet::Packet(Instructions::Instruction maininstr, std::string data)
{
	InputInstruction = maininstr;
	Data = data;
	//Data = (char*)malloc(sizeof(char) * 500);
	//if (Data == NULL) return;
	//strncpy(Data, data.c_str(), sizeof(Data));
	//strcpy_s(Data, sizeof(Data), data.c_str());
	//data.copy(Data, data.size());
	Begin = Instructions::Instruction::STARTPACKET;
	End = Instructions::Instruction::ENDPACKET;
}