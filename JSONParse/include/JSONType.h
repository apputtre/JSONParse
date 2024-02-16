# ifndef JSONTYPE_H
# define JSONTYPE_H

#include <variant>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <typeinfo>
#include <format>

#include "JSONException.h"

namespace jsonparse
{

	class JSONIllegalTypeConversion : public JSONException {
	public:
		JSONIllegalTypeConversion(std::string type_from, std::string type_to)
			: JSONException(std::format("JSON type conversion error: attempted illegal conversion from {} to {}", type_from, type_to))
		{}
	};

	class JSONIllegalTypeInitialization : public JSONException {
	public:
		JSONIllegalTypeInitialization(std::string type)
			: JSONException(std::format("JSON type initialization error: attempted illegal initialization of JSONType with type {}", type))
		{}
	};

	class JSONIllegalIndexOperation : public JSONException {
	public:
		JSONIllegalIndexOperation(std::string obj_type, std::string key_type)
			: JSONException(std::format("JSON illegal index operation: attempted to index JSONType of type {} using a key of type {}", obj_type, key_type))
		{}
	};

	class JSONType;
	typedef std::vector<JSONType> JSONArray;
	typedef std::map<std::string, JSONType> JSONObject;

	enum class JSONBool
	{
		True = 1,
		False = 0
	};

	class JSONType
	{
	public:
		JSONType()
			: tag{ TypeTag::Null }
		{}

		JSONType(int val)
			: val{ val }, tag{ TypeTag::Number }
		{}

		JSONType(double val)
			: val{ val }, tag{ TypeTag::Number }
		{}

		JSONType(std::string val)
			: val{ val }, tag{ TypeTag::String }
		{}

		JSONType(JSONBool val)
			: val{ val }, tag{ TypeTag::Boolean }
		{}

		JSONType(JSONArray val)
			: val{ val }, tag{ TypeTag::Array }
		{}

		JSONType(JSONObject val)
			: val{ val }, tag{ TypeTag::Object }
		{}

		template<typename T>
		T get();

		explicit operator bool();

		std::string serialize(bool prettyprint = true);
		const type_info& getType();

		template<size_t N>
		JSONType& operator=(const char(&val)[N]);

		JSONType& operator[](std::string key);
		JSONType& operator[](size_t index);

		friend std::ostream& operator<<(std::ostream& os, JSONType obj);

	private:

		enum class TypeTag
		{
			Number,
			String,
			Boolean,
			Array,
			Object,
			Null
		};

		TypeTag tag;

		std::variant<
			std::monostate,
			int,
			double,
			std::string,
			JSONBool,
			JSONArray,
			JSONObject
		> val;

	};

}

#include "JSONType.tpp"

# endif
