#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "jsonparse.h"
#include "JSONType.h"
#include "JSONNumber.h"
#include "JSONObject.h"
#include "tests.h"

using namespace jsonparse;

int main()
{
	JSONObject jobj;

	jobj["a"] = JSONNumber(5);

	JSONNumber jt(5);

	std::cout << jt << "\n";
}

