#ifdef JSONTYPE_H

using namespace jsonparse;

template<size_t N>
JSONType& JSONType::operator=(const char (&val)[N])
{
	this->val = std::string(val);
	this->tag = TypeTag::String;
	return *this;
}

#endif