#pragma once
#include <algorithm>
#include <functional>
#include "BasicClass.h"

namespace jbon {
	class GenericClass :
		public BasicClass
	{
	private:
		ObjectCollection objects;
		//unique function, that simply adds an object to the collection
		ClassObject& addObject(const ClassObject& object); //no one can make a classObject besides GenericClass, so private
		void reserve(size_t size);
	public:
		//constructor and yea
		GenericClass(std::string_view schema);
		GenericClass();

		//get the associated objects
		ObjectCollection& getObjects();

		//serialization/string functions
		std::string serialize() const;
		std::string serializeBody() const;
		friend std::ostream& operator<< (std::ostream& stream, GenericClass& genericClass);
		void makeAndAddObject(std::string_view data); //parses through data to add object

		//functions for making new object
		template<typename...args>
		ClassObject& constructObject(args...list) {
			if (keys.size() != sizeof...(list)) {
				std::cerr << "Generic Class " << className << " has " << keys.size() <<
					" parameters, but was only constructed with " << sizeof...(list) << std::endl;
				throw;
			}
			ClassObject classObject;
			push_backOnObject(classObject, 0, list...);
			return addObject(classObject);
		}
		template<typename...args>
		ClassObject& constructNamedObject(std::string name, args...list) {
			auto& object = constructObject(list...);
			object.objectName = std::make_unique<std::string>(name);
			return object;
		}
		ClassObject& constructObject(std::vector<Value> values);
		ClassObject& constructNamedObject(std::string name, std::vector<Value> values);
		
		//some sneaky template garbage
	private:
		template<typename T>
		void push_backOnObject(ClassObject& object, int key, T value) {
			object.push_back(keys.at(key), value);
		}
		//allows to add parameter packs to an object
		template<typename T, typename...args>
		void push_backOnObject(ClassObject& object, int key, T value, args...list){
			object.push_back(keys.at(key), value);
			push_backOnObject(object, ++key, list...);
		}
	};
}
