#pragma once
#include <variant>
#include <string>
#include <unordered_map>
#include "value.h"
#include "ValueType.h"
#include <iostream>
#include "ObjectType.h"
#include <optional>

namespace jbon {
	
	class ClassObject 
		: public ObjectType 
	{
		friend class BasicClass; //so it can serialize these puppies
		friend class GenericClass; //push fields back
	protected:
		//names are optional
		std::optional<std::string> objectName;
		//serialization method
		std::string serialize(std::string className) const;
		std::string getPrintable() const; //this it without the name
	public:
		ClassObject();
		//moving overloads for unique_ptr
		ClassObject(const ClassObject& other);
		ClassObject& operator=(const ClassObject&);

		Value const& operator[](std::string const& key) {
			auto it = fields.find(key);
			if (it == fields.end()) {
				std::cerr << "Invalid Key Search:\""<<key<<"\""<< std::endl;
				throw "Invalid Key Search";
			}
			return it->second;
		}
		//how to print out
		std::string name() const;
		void addName(std::string const&);
		friend std::ostream& operator << (std::ostream& stream, const ClassObject& object);
	};
}

