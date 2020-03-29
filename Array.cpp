#include "pch.h"
#include "Array.h"
#include "StringFunctions.h"

namespace jbon {
	Array * Array::create(std::string str)
	{
		return new Array(str);
	}
	bool Array::isSameType(std::string str)
	{
		return str.at(0) == '[' && str.at(str.size() - 1) == ']';
	}
	std::pair<char, char> Array::getSurroundingDelimeters()
	{
		return std::pair<char, char>('[',']');
	}
	Array::Array(std::string str)
	{
		auto textValues = jbonSplit(str.substr(1, str.size() - 1), ',');
		for (auto textValue : textValues) {
			values.push_back(Value::parse(textValue));
		}
	}
	Array::Array() {}
	Array::~Array() {}

	std::string Array::serialize()
	{
		std::string body = "";
		for (auto& value : values) {
			body += value.serialize() + ", ";
		}
		return "[" + body + "]";
	}
}