#pragma once
#include <algorithm>
#include <string>
#include <type_traits>
static class Instructions {
public:
	static enum class Instruction { NEWTABLE, GETVALUE, SETVALUE,
									SWITCHTABLE, EXIT, SHUTDOWN, 
									STARTPACKET, ENDPACKET, TESTPACKET, UNRECOGNIZED };
	template <typename E>
	static constexpr auto to_underlying(E e) noexcept
	{
		return static_cast<std::underlying_type_t<E>>(e);
	}
};