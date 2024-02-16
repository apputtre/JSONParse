# ifndef JSONPARSE_H
# define JSONPARSE_H

#include <sstream>

#include "JSONParser.h"
#include "JSONType.h"

namespace jsonparse
{

	JSONType parse(std::istream& is);
}

# endif JSONPARSE_H