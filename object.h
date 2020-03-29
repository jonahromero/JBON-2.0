#pragma once
#include <unordered_map>
#include "value.h"
#include "ClassObject.h"
#include "ValueType.h"
#include <string>
#include <vector>
//might be an error because copying over iterators
namespace jbon {
	class Object :
		public ClassObject,
		public ValueTypeContract<Object>
	{
		///friend class ;
	private:
		friend class ValueTypeContract<Object>;
		friend class ValueRegistration<Object>;		
		static bool isSameType(std::string str);
		static Object * create(std::string str);
		static std::pair<char, char> getSurroundingDelimeters();
		//everything has to be a pointer because thats what's used
	public:
		
		Object();
		Object(std::string input);
		~Object();
		//copying
		Object(const Object& other);
		Object& operator=(const Object& other);
		//serializable functions
		std::string serialize();
		template <typename T>
		void emplace(std::string key, T value) {
			push_back(key, Value::make<T>(value));
		}
		void erase(std::string key);
		void print() override;
	};
}

