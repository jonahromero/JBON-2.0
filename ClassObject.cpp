#include "pch.h"
#include "ClassObject.h"
#include "Parser.h"
#include <iostream>
#include "StringFunctions.h"
//serialization
namespace jbon {
	std::string ClassObject::serialize(std::string className) const
	{
		std::string heading = className;
		if (objectName.has_value()) {
			heading += ":"+ objectName.value();
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

		objectName = other.objectName;
	}

	ClassObject& ClassObject::operator=(const ClassObject& other)
	{
		fields.clear();
		insertOrder.clear();
		for (auto it : other.insertOrder) {
			push_back(it->first, it->second);
		}
		objectName = other.objectName;
		return *this;
	}

	ClassObject::ClassObject() {}
	std::string jbon::ClassObject::name() const
	{
		return objectName ? *objectName : "";
	}
	void ClassObject::addName(std::string const & name)
	{
		if (!objectName.has_value()) {
			objectName = name;
		}
	}
	std::string ClassObject::getPrintable() const
	{
		std::string heading = "";
		if (objectName.has_value()) {
			heading += objectName.value();
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
		return heading + "(\n" + body + "\n)\n";
	}

	std::ostream & operator<<(std::ostream & stream, const ClassObject & object)
	{
		stream << object.getPrintable();
		return stream;
	}
}