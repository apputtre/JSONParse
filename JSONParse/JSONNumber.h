#ifndef JSONNUMBER_H
#define JSONNUMBER_H

#include "JSONType.h"

namespace jsonparse
{

class JSONNumber : public JSONType
{
public:
	JSONNumber(int val)
		: val{ val }
	{};

	operator int();
private:
	int val;
};

}


#endif