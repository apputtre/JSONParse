#include "tests.h"

#include "jsonparse.h"

using namespace jsonparse;

std::string tests::report;

int tests::assert(bool b, size_t linenum)
{
	if (!b)
	{
		report += std::format("Failed test on line {}\n", linenum);
		return 1;
	}

	return 0;
}

#define ASSERT(x) tests::assert(x, __LINE__)

int tests::run_tests()
{
	report.clear();
	bool failure = 0;

	// initialzing JSONTypes / getting values
	{
		JSONType jt1 = 15;
		failure |= ASSERT(jt1.get<int>() == 15);
		JSONType jt2 = 2.5;
		failure |= ASSERT(jt2.get<double>() == 2.5);
		JSONType jt3 = JSONBool::True;
		failure |= ASSERT(jt3.get<JSONBool>() == JSONBool::True);
		JSONType jt4 = JSONArray();
		failure |= ASSERT(jt4.getType() == typeid(JSONArray));
		JSONType jt5 = JSONObject();
		failure |= ASSERT(jt5.getType() == typeid(JSONObject));
		JSONType jt6 = std::string("JSON");
		failure |= ASSERT(jt6.get<std::string>() == "JSON");
	}

	// storing elements in arrays

	// storing elements in objects

	// parsing arrays

	// parsing objects

	// serializing arrays

	// serializing objects

	if (!failure)
		report += "All tests passed";

	return failure;

}