#include "pch.h"
#include "ClassObject.h"
#include "Parser.h"
#include <iostream>
#include "StringFunctions.h"
//serialization
namespace jbon {
	void ClassObject::push_back(std::string key, Value value)
	{
		auto it = this->fields.emplace(key, value);
		insertOrder.push_back(it.first);
	}
	std::string ClassObject::serialize(std::string className)
	{
		std::string heading = className;
		if (objectName) {
			heading += ":"+ *objectName;
		}
		//only serializes values in map
		std::string body = "";
		for (auto& it : insertOrder) {
			body += it->second.serialize() + ",\n";
		}
		if (body.size() == 2) {
			std::cerr << "Invalid number of parameters in an object of " << className << std::endl;
		}
		body = body.substr(0, body.size() - 2);
		indent(body, 4);
		//using substring in order to get rid of, last comma and space
		return heading + "(\n" + body + "\n)\n";
	}

	ClassObject::ClassObject(const ClassObject& other)
	{
		//use the emplace method 
		for (auto it : other.insertOrder) {
			push_back(it->first, it->second);
		}
		if (other.objectName) {
			objectName = std::make_unique<std::string>(*other.objectName);
		}
	}

	ClassObject& ClassObject::operator=(const ClassObject& other)
	{
		fields.clear();
		insertOrder.clear();
		for (auto it : other.insertOrder) {
			push_back(it->first, it->second);
		}
		if (other.objectName) {
			objectName = std::make_unique<std::string>(*other.objectName);
		}
		return *this;
	}

	ClassObject::ClassObject() {}
	ClassObject::~ClassObject() {}
	std::string jbon::ClassObject::name()
	{
		return objectName ? *objectName : "";
	}
	void ClassObject::print()
	{
		std::string heading = "";
		if (objectName) {
			heading += *objectName;
		}
		std::string body = "";
		for (auto& it : insertOrder) {
			body += it->first + " = " + it->second.serialize() + ",\n";
		}
		if (body.size() == 2) {
			std::cerr << "Invalid number of parameters in an object" << std::endl;
		}
		body = body.substr(0, body.size() - 2);
		indent(body, 4);
		//using substring to get rid of last comma and space
		std::cout<< heading + "(\n" + body + "\n)\n"<<std::endl;
	}

	bool ClassObject::contains(std::string key)
	{
		return fields.count(key) > 0;
	}
	int ClassObject::size()
	{
		return fields.size();
	}
}