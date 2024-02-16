#include "JSONParser.h"

#include <sstream>

using namespace jsonparse;

JSONType JSONParser::parse()
{
	nextToken();
	JSONType ret = evaluate_token(current_token);
	return ret;
}

void JSONParser::nextToken()
{
	token_buffer.clear();

	char c = is.get();

	// skip white space
	while (std::isspace(c))
		c = is.get();

	// token is a string
	if (c == '\"')
	{
		while ((c = is.get()) != '\"')
		{
			if (c == EOF)
				throw JSONUnexpectedChar("EOF", is.tellg());

			token_buffer += c;
		}

		current_token = Token::STRING;
		return;
	}
	// token is a number
	else if (std::isdigit(c) || c == '.' || c == '-')
	{
		bool foundDecimal = false;

		bool exit = false;
		while (!exit)
		{
			if (c == '-')
			{
				if (!isdigit(is.peek()) && (is.peek() != '.'))
					throw JSONUnexpectedChar(is.peek(), is.tellg());
				token_buffer += c;
				c = is.get();
			}
			// e.g. .25, -.5 (not strictly permissible in the JSON standard)
			if (c == '.')
			{
				if (!std::isdigit(is.peek()))
					throw JSONUnexpectedChar(is.peek(), is.tellg());
				foundDecimal = true;
				token_buffer += c;
				c = is.get();
			}
			while (isdigit(c))
			{
				token_buffer += c;
				c = is.get();
				// decimal part
				if (c == '.' && !foundDecimal)
				{
					if (!std::isdigit(is.peek()))
						throw JSONUnexpectedChar(is.peek(), is.tellg());
					foundDecimal = true;
					token_buffer += c;
					c = is.get();
				}
			}
			// exponent part
			if ((c == 'E' || c == 'e'))
			{
				token_buffer += c;
				c = is.get();
				// exponents can be prefaced by a '-', '+', or no sign
				if (c == '+' || c == '-')
				{
					if (!isdigit(is.peek()))
						throw JSONUnexpectedChar(is.peek(), is.tellg());
					token_buffer += c;
					c = is.get();
				}
				while (isdigit(c))
				{
					token_buffer += c;
					c = is.get();
				}

			}

			exit = true;
		}
		is.unget();

		current_token = Token::NUMBER;
		return;
	}
	// token may be true
	else if (c == 't')
	{
		for (char ch : {'r', 'u', 'e'})
			if ((c = is.get()) != ch)
				throw JSONUnexpectedChar(c, is.tellg());
		current_token = Token::TRUE;
	}
	// token may be false
	else if (c == 'f')
	{
		for (char ch : {'a', 'l', 's', 'e'})
			if ((c = is.get()) != ch)
				throw JSONUnexpectedChar(c, is.tellg());
		current_token = Token::FALSE;
	}
	// token may be null
	else if (c == 'n')
	{
		for (char ch : {'u', 'l', 'l'})
			if ((c = is.get()) != ch)
				throw JSONUnexpectedChar(c, is.tellg());
		current_token = Token::NULL_TOKEN;
	}
	else if (c == '{')
		current_token = Token::LEFT_CURLY_BRACE;
	else if (c == '}')
		current_token = Token::RIGHT_CURLY_BRACE;
	else if (c == '[')
		current_token = Token::LEFT_SQUARE_BRACKET;
	else if (c == ']')
		current_token = Token::RIGHT_SQUARE_BRACKET;
	else if (c == ':')
		current_token = Token::COLON;
	else if (c == ',')
		current_token = Token::COMMA;
	else if (c == EOF)
	{
		is.unget();
		current_token = Token::END;
	}
	else
	{
		throw JSONUnexpectedChar(c, is.tellg());
	}
}

double JSONParser::stringToDouble(std::string str)
{
	return std::stod(str);
}

std::string JSONParser::doubleToString(double val)
{
	return std::to_string(val);
}

JSONType JSONParser::evaluate_token(Token t)
{
	switch (t)
	{
	case (Token::LEFT_CURLY_BRACE):
		return create_object();
		break;
	case (Token::LEFT_SQUARE_BRACKET):
		return create_array();
		break;
	case (Token::NUMBER):
	{
		double val = stringToDouble(token_buffer);
		if ((int) val == val)
			return (int)val;
		else
			return val;
		break;
	}
	case (Token::STRING):
		return token_buffer;
		break;
	case (Token::TRUE):
		return JSONBool::True;
		break;
	case (Token::FALSE):
		return JSONBool::False;
		break;
	case (Token::NULL_TOKEN):
		return JSONType();
		break;
	default:
		throw JSONUnexpectedToken(to_string(t));
		break;
	}
}

JSONObject JSONParser::create_object()
{
	if (current_token != Token::LEFT_CURLY_BRACE)
		throw JSONUnexpectedToken(to_string(current_token), to_string(Token::LEFT_CURLY_BRACE));

	JSONObject obj;

	nextToken();
	while (current_token != Token::RIGHT_CURLY_BRACE)
	{
		if (current_token != Token::STRING)
			throw JSONUnexpectedToken(to_string(current_token), to_string(Token::STRING));

		object_key_insertion(obj);
		nextToken();

		if (current_token == Token::COMMA)
			nextToken();
		else if (current_token != Token::RIGHT_CURLY_BRACE)
			throw JSONUnexpectedToken(to_string(current_token), to_string(Token::RIGHT_CURLY_BRACE));
	}

	return obj;
}

JSONArray JSONParser::create_array()
{
	if (current_token != Token::LEFT_SQUARE_BRACKET)
		throw JSONUnexpectedToken(to_string(current_token), to_string(Token::LEFT_SQUARE_BRACKET));

	JSONArray arr;

	nextToken();
	while (current_token != Token::RIGHT_SQUARE_BRACKET)
	{
		array_element_insertion(arr);
		nextToken();

		if (current_token == Token::COMMA)
			nextToken();
		else if (current_token != Token::RIGHT_SQUARE_BRACKET)
			throw JSONUnexpectedToken(to_string(current_token), to_string(Token::RIGHT_SQUARE_BRACKET));
	}

	return arr;
}

void JSONParser::object_key_insertion(JSONObject& obj)
{
	if (current_token != Token::STRING)
		throw JSONUnexpectedToken(to_string(current_token), to_string(Token::STRING));

	std::string key = token_buffer;

	nextToken();
	if (current_token != Token::COLON)
		throw JSONUnexpectedToken(to_string(current_token), to_string(Token::COLON));
	nextToken();

	obj[key] = evaluate_token(current_token);
}

void JSONParser::array_element_insertion(JSONArray& arr)
{
	arr.push_back(evaluate_token(current_token));
}

std::string JSONParser::to_string(const Token& t)
{
	std::stringstream ss;
	ss << t;
	return ss.str();
}

std::ostream& jsonparse::operator<<(std::ostream& os, JSONParser::Token t)
{
	using Token = JSONParser::Token;

	switch (t)
	{
	case(Token::STRING):
		os << "STRING";
		break;
	case(Token::NUMBER):
		os << "NUMBER";
		break;
	case(Token::TRUE):
		os << "TRUE";
		break;
	case(Token::FALSE):
		os << "FALSE";
		break;
	case(Token::NULL_TOKEN):
		os << "NULL_TOKEN";
		break;
	case(Token::LEFT_CURLY_BRACE):
		os << "{";
		break;
	case(Token::RIGHT_CURLY_BRACE):
		os << "}";
		break;
	case(Token::LEFT_SQUARE_BRACKET):
		os << "[";
		break;
	case(Token::RIGHT_SQUARE_BRACKET):
		os << "]";
		break;
	case(Token::COLON):
		os << ":";
		break;
	case(Token::COMMA):
		os << ",";
		break;
	case(Token::END):
		os << "END";
		break;
	}

	return os;
}