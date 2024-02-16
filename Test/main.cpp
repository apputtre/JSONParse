#include <fstream>

#include "jsonparse.h"

int main()
{
	std::fstream file("test.json");
	jsonparse::JSONType jt = jsonparse::parse(file);
	std::cout << jt["a"] << "\n";
}