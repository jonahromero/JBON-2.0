#include "pch.h"
#include "GenericClass.h"
#include "Parser.h"
#include <memory>
#include <iostream>
#include "StringFunctions.h"
#include "value.h"
#include "ClassObject.h"

namespace jbon {
	std::string GenericClass::serialize()
	{
		//make class schema
		std::string classSchema = className,
					body = "";
		//for the keys, make a giant string of them
		for (int i = 0; i < keys.size(); i++) {
			if(i != keys.size() - 1)
				body += keys.at(i) + ",\n";
			else
				body += keys.at(i) + "\n";
		}
		//indent all the keys
		indent(body, 4);
		//add the closing brackets and attach to classSchema
		classSchema += " {\n" + body + "}\n";
		//serialize objects
		std::string objectString = "";
		for (auto& object : objects) {
			objectString += object.serialize(className);
		}
		return classSchema + objectString;
	}
	GenericClass::iterator GenericClass::find(std::string name)
	{
		return std::find_if(objects.begin(), objects.end(), std::bind([](std::string name, auto& object) ->bool {
			if (object.name() == name) {
				return true;
			}
			return false;
		},name, std::placeholders::_1));
	}
	void GenericClass::erase(iterator it)
	{
		objects.erase(it);
	}
	int GenericClass::size()
	{
		return objects.size();
	}
	int GenericClass::count()
	{
		return objects.size();
	}
	bool GenericClass::empty()
	{
		return objects.empty();
	}
	GenericClass::GenericClass(std::string schema)
	{
		std::string body, field;
		removeSpaces(schema);
		splitTitleBody(schema, className, body, '{','}');
		keys = split(body.substr(1, body.size() - 2), ',');
	}

	GenericClass::~GenericClass(){}
	std::string GenericClass::name()
	{
		return className;
	}
	GenericClass::iterator GenericClass::constructObject(std::vector<Value> values)
	{
		if (keys.size() != values.size()) {
			std::cerr << "Generic Class " << className << " has " << keys.size() <<
				" parameters, but was only constructed with " << values.size() << std::endl;
			return end();
		}
		ClassObject classObject;
		for (int i = 0; i < keys.size(); i++) {
			classObject.push_back(keys.at(i), values.at(i));
		}
		objects.push_back(classObject);
		return objects.begin() + objects.size() - 1;
	}
	GenericClass::iterator GenericClass::constructObject(std::string name, std::vector<Value> values)
	{
		if (keys.size() != values.size()) {
			std::cerr << "Generic Class " << name << " has " << keys.size() <<
				" parameters, but was only constructed with " << values.size() << std::endl;
			return end();
		}
		ClassObject classObject;
		classObject.objectName = std::make_unique<std::string>(name);
		for (int i = 0; i < keys.size(); i++) {
			//std::cout << keys.at(i) << ":" << values.at(i).serialize() << std::endl;
			classObject.push_back(keys.at(i), values.at(i));
		}
		objects.push_back(classObject);
		return objects.begin() + objects.size() - 1;
	}
}
