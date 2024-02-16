#ifndef JSONOBJECT_H
#define JSONOBJECT_H

#include <map>
#include <string>

#include "JSONType.h"

class JSONObject : public JSONType
{
private:
	typedef std::map<std::string, JSONType*> _storageType;
public:
	JSONType& operator[](std::string key);

	std::string serialize();
private:
	_storageType vals;
};

#endif