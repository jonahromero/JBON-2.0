#include "pch.h"
#include "Array.h"
#include "StringFunctions.h"

namespace jbon {
	Array * Array::create(std::string_view str)
	{
		return new Array(str);
	}
	bool Array::isSameType(std::string_view str)
	{
		return str.at(0) == '[' && str.at(str.size() - 1) == ']';
	}
	std::pair<char, char> Array::getSurroundingDelimeters()
	{
		return std::pair<char, char>('[', ']');
	}
	Array::Array(std::string_view str)
	{
		std::vector<std::string_view> fields;
		jbonSplit(fields, str.substr(1, str.size() - 2), ',');
		values.reserve(fields.size());
		for (auto& value : fields) {
			values.push_back(Value::parse(value));
		}
	}

	std::string Array::serialize() const{
		std::string body = "";
		for (auto& value : values) {
			body += value.serialize() + ", ";
		}
		return "[" + body.substr(0, body.size() - 2) + "]";
	}
}