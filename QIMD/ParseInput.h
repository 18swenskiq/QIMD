#pragma once
#include <algorithm>
#include <string>
#include "Instructions.h"

class ParseInput {
public:
	Instructions::Instruction GetInstruction(std::string input);
};