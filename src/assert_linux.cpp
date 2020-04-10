#ifndef WIN32
#include "assert.hpp"

#include <iostream>

namespace testing
{

bool assert(const std::string& expr, const std::string& file, unsigned int line)
{
	std::cerr << "Assertion:   " << expr << "\n\n" << "File:" << file << "(" << line << ")." << std::endl;
	std::cerr << "Interrupting, if gdb attached it should catch it!" << std::endl;
	return true;
}

}
#endif // WIN32
