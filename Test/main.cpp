#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "jsonparse.h"
#include "JSONType.h"
#include "tests.h"

using namespace jsonparse;

int main()
{
	tests::run_tests();
	std::cout << tests::report;
}

