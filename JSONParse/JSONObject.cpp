#include "jsonparse.h"
#include "JSONObject.h"

JSONType& JSONObject::operator[](std::string key)
{
	return *vals[key];
}

std::string JSONObject::serialize()
{
	return "";
}
