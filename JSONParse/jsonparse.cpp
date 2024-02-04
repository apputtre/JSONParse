#include "jsonparse.h"

using namespace jsonparse;

JSONType jsonparse::parse(std::istream& is)
{
	JSONParser p(is);
	return p.parse();
}
