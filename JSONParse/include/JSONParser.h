#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "JSONException.h"
#include "JSONType.h"

namespace jsonparse
{

	class JSONLexerError : public JSONException
	{
	public:
		JSONLexerError(std::string msg)
			: JSONException("JSON lexer error: " + msg)
		{}
	};

	class JSONUnexpectedChar : public JSONLexerError
	{
	public:
		JSONUnexpectedChar(char c, unsigned int pos)
			: JSONLexerError(std::format("Unexpected character \'{}\' encounted at position {}", c, pos))
		{}

		JSONUnexpectedChar(std::string str, unsigned int pos)
			: JSONLexerError(std::format("Unexpected character \'{}\' encounted at position {}", str, pos))
		{}

		/*
		JSONUnexpectedChar(char c, unsigned int pos, std::string file)
			: JSONLexerError(std::format("Unexpected character \'{}\' encountered at position {} while lexing file {}", c, pos, file))
		{}
		*/
	};

	class JSONParserError : public JSONException
	{
	public:
		JSONParserError(std::string msg)
			: JSONException("JSON parser error: " + msg)
		{}
	};

	class JSONUnexpectedToken : public JSONParserError
	{
	public:
		JSONUnexpectedToken(std::string found, std::string expected)
			: JSONParserError(std::format("Unexpected token {} encountered; expected {}", found, expected))
		{}

		JSONUnexpectedToken(std::string found)
			: JSONParserError(std::format("Unexpected token {} encountered", found))
		{}
	};


	class JSONParser
	{
	public:
		JSONParser(std::istream& is)
			: is{ is }, current_token{ Token::NULL_TOKEN }, token_buffer{ "" }
		{}

		JSONType parse();

		enum class Token
		{
			NUMBER,
			STRING,
			LEFT_CURLY_BRACE = '{',
			RIGHT_CURLY_BRACE = '}',
			LEFT_SQUARE_BRACKET = '[',
			RIGHT_SQUARE_BRACKET = ']',
			COLON = ':',
			COMMA = ',',
			NULL_TOKEN,
			TRUE,
			FALSE,
			END = -1
		};

	private:
		std::istream& is;

		// drives the lexer
		void nextToken();
		// convert a string of numbers of the form recognized by the lexer into a decimal value
		double stringToDouble(std::string str);
		// convert a decimal value into a string of numbers of the form recognized by the lexer
		std::string doubleToString(double val);


		JSONType evaluate_token(Token t);
		JSONObject create_object();
		JSONArray create_array();
		void object_key_insertion(JSONObject& obj);
		void array_element_insertion(JSONArray& arr);

		std::string to_string(const Token& t);

		friend std::ostream& operator<<(std::ostream& os, Token t);

		// keeps track of which character the lexer is reading (for debugging purposes)
		Token current_token;
		std::string token_buffer;

	};

};	//namespace jsonparse


#endif