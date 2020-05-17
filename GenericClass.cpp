#include "pch.h"
#include "GenericClass.h"
#include "Parser.h"
#include <memory>
#include <iostream>
#include "StringFunctions.h"
#include "value.h"
#include "ClassObject.h"
#include<optional>
namespace jbon {
	ObjectCollection & GenericClass::getObjects()
	{
		return objects;
	}

	std::string GenericClass::serialize() const
	{
		return BasicClass::serialize() + serializeBody();
	}

	std::string GenericClass::serializeBody() const
	{
		//serialize objects
		std::string objectString = "";
		for (const auto& object : objects) {
			objectString += object.serialize(className);
		}
		return objectString;
	}

	ClassObject & GenericClass::addObject(const ClassObject & object)
	{
		objects.objects.push_back(object);
		return objects.objects[objects.size() - 1];
	}

	void GenericClass::reserve(size_t size)
	{
		objects.objects.reserve(size);
	}

	GenericClass::GenericClass(std::string_view schema) : BasicClass(schema) {}
	GenericClass::GenericClass() {}

	void GenericClass::makeAndAddObject(std::string_view object)
	{
		std::optional<std::string> name;
		std::vector<Value> values;
		parseObject(values, object, name);
		if (name) {
			constructNamedObject(name.value(), values);
		}
		else {
			constructObject(values);
		}
	}
	ClassObject& GenericClass::constructObject(std::vector<Value> values)
	{
		if (keys.size() != values.size()) {
			std::cerr << "Generic Class " << className << " has " << keys.size() <<
				" parameters, but was only constructed with " << values.size() << std::endl;
			throw;
		}
		ClassObject classObject;
		for (int i = 0; i < keys.size(); i++) {
			classObject.push_back(keys.at(i), values.at(i));
		}
		//accesing objects in our ObjectCollection
		return addObject(classObject);
	}
	ClassObject& GenericClass::constructNamedObject(std::string name, std::vector<Value> values)
	{
		auto& object = constructObject(values);
		object.addName(name);
		return object;
	}
	std::ostream & operator<<(std::ostream & stream, GenericClass & genericClass)
	{
		stream << genericClass.serialize();
		return stream;
	}
}
