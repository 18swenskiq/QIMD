#include "ParseInput.h"
#include <iostream>

Instructions::Instruction ParseInput::GetInstruction(std::string input) {
	std::string token = input.substr(0, input.find(" "));

	std::transform(token.begin(), token.end(), token.begin(), ::toupper);
	if (token == "NEWTABLE")
	{
		return Instructions::Instruction::NEWTABLE;
	}
	else if (token == "GETVALUE")
	{
		return Instructions::Instruction::GETVALUE;
	}
	else if (token == "SETVALUE")
	{
		return Instructions::Instruction::SETVALUE;
	}
	else if (token == "SWITCHTABLE")
	{
		return Instructions::Instruction::SWITCHTABLE;
	}
	else if (token == "EXIT")
	{
		return Instructions::Instruction::EXIT;
	}
	else if (token == "SHUTDOWN")
	{
		return Instructions::Instruction::SHUTDOWN;
	}
	else
	{
		return Instructions::Instruction::UNRECOGNIZED;
	}
}