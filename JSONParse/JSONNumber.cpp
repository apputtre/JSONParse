#include "JSONNumber.h"
#include "jsonparse.h"

namespace jsonparse
{
	JSONNumber::operator int()
	{
		return val;
	}
}
