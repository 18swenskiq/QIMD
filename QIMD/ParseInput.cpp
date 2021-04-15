#include "ParseInput.h"

Instructions::Instruction ParseInput::GetInstruction(std::string input) {
	std::string token = input.substr(0, input.find(" "));

	std::transform(input.begin(), input.end(), input.begin(), ::toupper);
	if (input == "NEWTABLE")
	{
		return Instructions::Instruction::NEWTABLE;
	}
	else if (input == "GETVALUE")
	{
		return Instructions::Instruction::GETVALUE;
	}
	else if (input == "SETVALUE")
	{
		return Instructions::Instruction::SETVALUE;
	}
	else if (input == "SWITCHTABLE")
	{
		return Instructions::Instruction::SWITCHTABLE;
	}
	else if (input == "EXIT")
	{
		return Instructions::Instruction::EXIT;
	}
	else if (input == "SHUTDOWN")
	{
		return Instructions::Instruction::SHUTDOWN;
	}
	else
	{
		return Instructions::Instruction::UNRECOGNIZED;
	}
}