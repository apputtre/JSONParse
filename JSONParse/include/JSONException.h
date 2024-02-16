#ifndef JSONEXCEPTION_H
#define JSONEXCEPTION_H

#include <string>

class JSONException : public std::exception
{
public:
	JSONException(std::string msg)
		: msg{ msg } {}

	std::string what()
	{
		return msg;
	}

protected:
	std::string msg;
};

#endif
