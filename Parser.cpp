#include "pch.h"
#include "Parser.h"
#include <iostream>
#include "StringFunctions.h"
#include <type_traits>
#include "object.h"
#include <optional>
//typedef std::variant<int, float, bool, std::string, Object> value_t;

namespace jbon {
	Parser::Parser(std::string headers, std::string data) {
		//remove any spacing
		removeSpaces(headers);
		removeSpaces(data);
		//parse through the headers
		auto genericClasses = splitClasses(headers);
		for (auto& genericClass : genericClasses) {
			//temporary so we can use itself to get the name
			GenericClass temp = GenericClass(genericClass);
			classes.emplace(temp.name(), temp);
		}
		//parse through the constructor data
		auto classObjects = splitClassObjects(data);
		for (auto& classObject : classObjects) {
			//variables
			std::string title, body;
			std::optional<std::string> objectName;

			splitTitleBody(classObject, title, body, '(', ')');

			
			//see if it has its own name, otherwise nothing changes
			for (int i = 0; i < title.size(); i++) {
				if (title.at(i) == ':') { //colon is used to specify name
					objectName = title.substr(i + 1, title.size() - i + 1);
					title = title.substr(0, i);
					//Dog:Scruffles -> title = Dog, objectName = Scruffles
				}
			}
			//get all of the values from data
			GenericClass& genericClass = classes.at(title);
			std::vector<Value> values;
			//makes all the class objects from file by parsing values
			std::vector<std::pair<char, char>> delimeters = *Registrants::getDelimeters();
			delimeters.push_back({ '\"','\"' }); //all the other delimeters should be defined in their respective class
			auto fields = split(body.substr(1, body.size() - 2), delimeters, ',');

			for (auto& field : fields) {
				values.push_back(Value::parse(field));
			}
			if (objectName) {
				genericClass.constructObject(objectName.value(), values);
			}
			else {
				genericClass.constructObject(values);
			}
		}
	}
	GenericClass & Parser::getClass(std::string name)
	{
		return classes.at(name);
	}
	std::vector<GenericClass> Parser::getClasses()
	{
		std::vector<GenericClass> genVec;
		for (auto& genClass : classes)
			genVec.push_back(genClass.second);
		return genVec;
	}
	Parser::~Parser() {}
	std::string Parser::to_string()
	{
		std::string result = "";
		for (auto& [key, value] : classes) {
			result += value.serialize();
		}	
		return result;
	}
}