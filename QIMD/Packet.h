#pragma once
#include "Instructions.h"
class Packet {
public:
	Packet(Instructions::Instruction maininstr, std::string data);	
	Instructions::Instruction InputInstruction;
	std::string Data;	
private:
	Instructions::Instruction Begin;
	Instructions::Instruction End;
};