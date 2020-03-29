#include "pch.h"
#include "object.h"
#include "Parser.h"
#include "StringFunctions.h"
#include <vector>

namespace jbon {
	Object::Object()
	{
	}

	Object::Object(std::string input)
	{
		removeSpaces(input);
		//removes first and last character, as the format for an object is
		// {x=3, y=5}
		std::vector<std::pair<char,char>> delimeters = *Registrants::getDelimeters();
		delimeters.push_back({ '\"','\"' }); //all the other delimeters should be defined in their respective class
		auto fields = split(input.substr(1, input.size() - 2), delimeters, ',');
		for (auto& field : fields) {
			std::string key, value;
			for (int i = 0; i < field.size(); i++) {
				if (field.at(i) == '=') {
					key = field.substr(0, i);
					value = field.substr(i + 1, field.size() - (i + 1));
				}
			}
			push_back(key, Value::parse(value));
		}
	}

	Object::~Object()
	{
	}

	std::string Object::serialize()
	{
		std::string block = "";
		for (auto& it: insertOrder) {
			block += it->first + " = " + it->second.serialize() + "," + "\n";
		}
		block = block.substr(0, block.size() - 2);
		indent(block, 4); //will insert number of spaces after every new line
		return "{\n" + block + "\n}";
	}

	Object * Object::create(std::string str)
	{
		return new Object(str);
	}

	std::pair<char, char> Object::getSurroundingDelimeters()
	{
		return std::make_pair('{','}');
	}

	bool Object::isSameType(std::string str)
	{
		return str.at(0) == '{' && str.at(str.size() - 1) == '}';
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

	void Object::print()
	{
		std::cout << serialize() << std::endl;
	}

	Object::Object(const Object & other)
	{
		for (auto it : other.insertOrder) {
			push_back(it->first, it->second);
		}
		if (other.objectName) {
			objectName = std::make_unique<std::string>(*other.objectName);
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
}