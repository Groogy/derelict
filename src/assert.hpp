#pragma once

#include <string>
#include <csignal>

#undef assert

#define DEBUGGER_BREAK std::raise(SIGINT)

#define ASSERT(expr)													\
{																		\
	if(!(expr))															\
	{																	\
		bool debugBreak = testing::assert(#expr, __FILE__, __LINE__);	\
		if(debugBreak)													\
			DEBUGGER_BREAK;												\
	}																	\
}

namespace testing
{
	// Need to name it wierdly to avoid stupid windows macro being dumb
	bool assert(const std::string& expr, const std::string& file, unsigned int line);
}
