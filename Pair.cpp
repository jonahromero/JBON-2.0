#include "pch.h"
#include "Pair.h"
#include "StringFunctions.h"

namespace jbon {
	Pair * Pair::create(std::string_view str)
	{
		return new Pair(str);
	}
	bool Pair::isSameType(std::string_view str)
	{
		return str[0] == '<' && str[str.size() - 1] == '>';
	}
	std::pair<char, char> Pair::getSurroundingDelimeters()
	{
		return std::pair<char, char>('<', '>');
	}
	Pair::Pair(std::string_view str)
	{
		std::vector<std::string_view> textValues;
		jbonSplit(textValues, str.substr(1, str.size() - 2), ',');
		first = Value::parse(textValues[0]);
		second = Value::parse(textValues[1]);
	}
	std::string Pair::serialize() const
	{
		const auto first_serialized = first.serialize();
		const auto second_serialized = second.serialize();
		std::string result;
		result.reserve(first_serialized.size() + second_serialized.size() + 4);
		result.push_back('<');
		result.append(first_serialized);
		result.append(", ", 2);
		result.append(second_serialized);
		result.push_back('>');
		return result;
	}
	Pair::Pair(){}
	Pair::~Pair(){}
	void Pair::swap()
	{
		Value temp = first;
		first = second;
		second = temp;
	}
}