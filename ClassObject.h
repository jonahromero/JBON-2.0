#pragma once
#include <variant>
#include <string>
#include <unordered_map>
#include "value.h"
#include "ValueType.h"

namespace jbon {
	
	class ClassObject
	{
		friend class GenericClass;
	protected:
		std::unique_ptr<std::string> objectName;
		typedef std::unordered_map<std::string, Value> fields_t;
		fields_t fields;
		std::vector<fields_t::iterator> insertOrder;
		void push_back(std::string key, Value value); //properly adds pair to object
		std::string serialize(std::string className);
	public:
		//moving overloads for unique_ptr
		ClassObject(const ClassObject& other);
		ClassObject& operator=(const ClassObject&);
		ClassObject();
		virtual ~ClassObject();
		std::string name();
		//the at methods are to edit an exsisting type at a key
		template <typename T>
		T& at(std::string key) {
			auto it = fields.find(key);
			return it->second.get<T>();
		}
		//the set methods can change the value type at a field to what they want
		template <typename T>
		void set(std::string key, T value) {
			auto it = fields.find(key);
			it->second = Value::make(value);
		}
		bool contains(std::string key);
		int size();
		//helper functions
		virtual void print();
	};
}

