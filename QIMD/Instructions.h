#pragma once
#include <algorithm>
#include <string>
static class Instructions {
public:
	static enum class Instruction { NEWTABLE, GETVALUE, SETVALUE,
									SWITCHTABLE, EXIT, SHUTDOWN, 
									STARTPACKET, ENDPACKET, UNRECOGNIZED };
};