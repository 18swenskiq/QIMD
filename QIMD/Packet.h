#pragma once
#include "Instructions.h"
class Packet {
public:
	Packet();
	Packet(char rawdata[]);
	Packet(Instructions::Instruction maininstr, std::string data);	
	char* Serialize();
public:
	Instructions::Instruction InputInstruction;
	std::string Data;	
private:
	Instructions::Instruction Begin;
	Instructions::Instruction End;
};