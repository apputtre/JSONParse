#ifndef JSONTYPE_H
#define JSONTYPE_H

#include <string>

class JSONType
{
private:
	virtual std::string serialize() = 0;
}

#endif
