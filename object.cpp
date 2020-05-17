#include "pch.h"
#include "object.h"
#include "Parser.h"
#include "StringFunctions.h"
#include <vector>
namespace jbon {
	Object::Object()
	{
	}

	Object::Object(std::string_view input)
	{
		//removes first and last character, as the format for an object is
		//{x=3, y=5}
		std::vector<std::string_view> fields;
		jbonSplit(fields, input.substr(1, input.size() - 2), ',');

		for (auto& field : fields) {
			std::string key;
			std::string_view value;
			for (int i = 0; i < field.size(); i++) {
				if (field[i] == '=') {
					key = std::string{ field.substr(0, i) };
					value = field.substr(i + 1, field.size() - (i + 1));
				}
			}
			push_back(key, Value::parse(value));
		}
	}
	std::string Object::serialize() const
	{
		std::string block = "";
		for (auto& it: insertOrder) {
			block += it->first + " = " + it->second.serialize() + "," + "\n";
		}
		block = block.substr(0, block.size() - 2);
		indent(block, 4); //will insert number of spaces after every new line
		std::string result;
		result.reserve(4 + block.size());
		result.append("{\n",2);
		result.append(block);
		result.append("\n}", 2);
		return result;
	}

	std::string Object::serializeValues() const
	{
		std::string block;
		for (auto& it : insertOrder) {
			block.append(it->second.serialize());
			block.append(", ",2);
		}
		block = block.substr(0, block.size() - 2);
		//indent(block, 4); //will insert number of spaces after every new line
		//check if this is faster
		std::string result;
		result.reserve(4+block.size());
		result.push_back('{');
		result.append(block);
		result.push_back('}');
		return result;
	}

	Object * Object::create(std::string_view str)
	{
		return new Object(str);
	}

	std::pair<char, char> Object::getSurroundingDelimeters()
	{
		return std::make_pair('{','}');
	}

	bool Object::isSameType(std::string_view str)
	{
		return str[0] == '{' && str[str.size() - 1] == '}';
	}

	void Object::erase(std::string key)
	{
		//deletes iterator and the value in the map
		for (auto it = insertOrder.begin(); it != insertOrder.end(); it++) {
			if ((*it)->first == key) {
			insertOrder.erase(it);
			fields.erase(*it);
			}
		}
	}

	Object::Object(const Object & other)
	{
		insertOrder.reserve(other.insertOrder.size());
		for (auto it : other.insertOrder) {
			push_back(it->first, it->second);
		}
	}
	Object & Object::operator=(const Object & other)
	{
		fields.clear();
		insertOrder.clear();
		for (auto it : other.insertOrder) {
			push_back(it->first, it->second);
		}
		return *this;
	}
	std::ostream & operator<<(std::ostream & stream, const Object & object)
	{
		stream << object.serialize();
		return stream;
	}
}