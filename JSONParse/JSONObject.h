#ifndef JSONOBJECT_H
#define JSONOBJECT_H

#include <map>

#include "JSONType.h"

class JSONObject : public JSONType
{
private:
	typedef std::map<std::string, JSONType*> _storageType;
public:
	JSONType& operator[](std::string key);
private:
	_storageType vals;
};

#endif