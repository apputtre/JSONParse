#include <string>
#include <algorithm>

#include "JSONNumber.h"

namespace jsonparse
{
	JSONNumber::operator int()
	{
		return val;
	}

	std::string JSONNumber::serialize()
	{
		return std::to_string(val);
	}
}
