#pragma once
#include "Instructions.h"

class ParseInput {
public:
	Instructions::Instruction GetInstruction(std::string input);
};