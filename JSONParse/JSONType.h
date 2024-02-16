#ifndef JSONTYPE_H
#define JSONTYPE_H

#include <string>
#include <ostream>

class JSONType
{
public:

	friend std::ostream& operator<<(std::ostream& os, JSONType& t)
	{
		os << t.serialize();

		return os;
	}

	virtual std::string serialize() = 0;

private:

};


#endif
