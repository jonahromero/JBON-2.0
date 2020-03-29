#pragma once
#include "ClassObject.h"
#include <algorithm>
#include <functional>

namespace jbon {
	class GenericClass
	{

		std::string className;
		std::vector<std::string> keys; //all the keys from the schema
		std::vector<ClassObject> objects; //all the associated objects
	public:
		//iterators and containers
		typedef std::vector<ClassObject>::iterator iterator;
		typedef std::vector<ClassObject>::const_iterator const_iterator;
		
		iterator begin() { return objects.begin(); }
		iterator end() { return objects.end(); }

		iterator find(std::string name); //finds a certain object based on name
		void erase(iterator it);
		int size();
		int count();
		bool empty();

		GenericClass(std::string schema);
		~GenericClass();
		//my functions
		std::string serialize();
		std::string name();
		iterator constructObject(std::vector<Value> values);
		iterator constructObject(std::string name, std::vector<Value> values);
	};
}
