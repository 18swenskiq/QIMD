#pragma once

static class Instructions {
public:
	static enum class Instruction { NEWTABLE, GETVALUE, SETVALUE,
									SWITCHTABLE, EXIT, SHUTDOWN, 
									STARTPACKET, ENDPACKET, UNRECOGNIZED };
};