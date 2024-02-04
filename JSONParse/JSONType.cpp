#include "JSONType.h"

#include <sstream>

using namespace jsonparse;

template<>
int JSONType::get<int>()
{
	if (tag == TypeTag::Number)
	{
		auto s = std::get_if<int>(&val);
		if (s)
			return *s;
		else
			return std::get<double>(val);
	}
	else
		throw JSONIllegalTypeConversion(getType().name(), typeid(int).name());
}

template<>
double JSONType::get<double>()
{
	if (tag == TypeTag::Number)
	{
		auto s = std::get_if<double>(&val);
		if (s)
			return *s;
		else
			return std::get<int>(val);
	}
	else
		throw JSONIllegalTypeConversion(getType().name(), typeid(double).name());
	
}

template<>
std::string JSONType::get<std::string>()
{
	if (tag == TypeTag::String)
		return std::get<std::string>(val);
	else
		throw JSONIllegalTypeConversion(getType().name(), typeid(std::string).name());
}

template<>
JSONBool JSONType::get<JSONBool>()
{
	if (tag == TypeTag::Boolean)
		return std::get<JSONBool>(val);
	else
		throw JSONIllegalTypeConversion(getType().name(), typeid(JSONBool).name());
}

template<>
JSONArray JSONType::get<JSONArray>()
{
	if (tag == TypeTag::Array)
		return std::get<JSONArray>(val);
	else
		throw JSONIllegalTypeConversion(getType().name(), typeid(JSONArray).name());
}

template<>
JSONObject JSONType::get<JSONObject>()
{
	if (tag == TypeTag::Object)
		return std::get<JSONObject>(val);
	else
		throw JSONIllegalTypeConversion(getType().name(), typeid(JSONObject).name());
}

JSONType::operator bool()
{
	if (tag == TypeTag::Boolean)
		return std::get<JSONBool>(val) == JSONBool::True;
	else if (tag == TypeTag::Null)
		return false;
	else if (tag == TypeTag::Number)
	{
		if (auto s = std::get_if<double>(&val))
			return *s > 0.0;
		else
			return std::get<int>(val) > 0;
	}
	else
		throw JSONIllegalTypeConversion(getType().name(), typeid(bool).name());
}

std::string JSONType::serialize(bool prettyprint)
{
	std::string newline = "\n";
	std::string tab = "\t";
	std::string sep = " ";

	if (!prettyprint)
	{
		newline = "";
		tab = "";
		sep = "";
	}

	switch (tag)
	{
	case (TypeTag::Null):
		return "null";
		break;
	case (TypeTag::Boolean):
		return std::get<JSONBool>(val) == JSONBool::True ? "true" : "false";
		break;
	case (TypeTag::String):
	{
		return "\"" + std::get<std::string>(val) + "\"";
		break;
	}
	case (TypeTag::Number):
	{
		auto s = std::get_if<int>(&val);
		if (s)
			return std::to_string(*s);
		else
			return std::to_string(std::get<double>(val));
	}
	case (TypeTag::Array):
	{
		std::string ret;

		JSONArray arr = this->get<JSONArray>();

		ret += "[";
		auto it = arr.begin();
		while (it != arr.end())
		{
			ret += (*it++).serialize(prettyprint);
			if (it != arr.end())
				ret = ret + "," + sep;
		}
		ret += "]";

		return ret;

		break;
	}
	case (TypeTag::Object):
	{
		JSONObject obj = this->get<JSONObject>();

		auto it = obj.begin();

		if (it == obj.end())
			return "{}";

		std::string ret = "{" + newline;

		while (it != obj.end())
		{
			ret = ret + "\"" + (*it).first + "\"" + sep + ":" + sep + (*it).second.serialize(prettyprint);
			if (++it != obj.end())
				ret = ret + "," + newline;
		}
		ret += newline + "}";

		if (prettyprint)
		{
			std::stringstream ss(ret);
			ss >> std::noskipws;
			ret.clear();

			char c;
			int indent_lvl = 0;
			while (ss >> c)
			{
				if (c == '{')
					++indent_lvl;
				if (ss.peek() == '}')
					indent_lvl = std::max(0, indent_lvl - 1);
				ret += c;

				if (c == '\n')
					for (int i = 0; i < indent_lvl; i++)
						ret += '\t';
			}
		}

		return ret;

		break;
	}
	}
}

JSONType& JSONType::operator[](std::string key)
{
	if (tag == TypeTag::Object)
	{
		JSONObject& ref = (JSONObject&)(std::get<JSONObject>(val));
		return ref[key];
	}
	else
		throw JSONIllegalIndexOperation(getType().name(), typeid(key).name());
}

JSONType& JSONType::operator[](size_t index)
{
	if (tag == TypeTag::Array)
	{
		JSONArray& ref = (JSONArray&)(std::get<JSONArray>(val));
		try
		{
			return ref.at(index);
		}
		catch (std::out_of_range e)
		{
			throw e;
		}

	}
	else
		throw JSONIllegalIndexOperation(getType().name(), typeid(index).name());
}

std::ostream& jsonparse::operator<<(std::ostream& os, JSONType obj)
{
	return os << obj.serialize();
}

const type_info& JSONType::getType()
{
	switch (tag)
	{
	case(TypeTag::Number):
		return typeid(double);
		break;
	case(TypeTag::String):
		return typeid(std::string);
		break;
	case(TypeTag::Boolean):
		return typeid(bool);
		break;
	case(TypeTag::Object):
		return typeid(JSONObject);
		break;
	case(TypeTag::Array):
		return typeid(JSONArray);
		break;
	case(TypeTag::Null):
		return typeid(NULL);
		break;
	}

}
