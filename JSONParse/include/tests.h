#ifndef TESTS_H
#define TESTS_H

#include <string>

namespace tests
{
	extern std::string report;

	int assert(bool b, size_t linenum);
	int run_tests();

}
#endif