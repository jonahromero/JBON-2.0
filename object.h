#pragma once
#include <unordered_map>
#include "value.h"
#include "ClassObject.h"
#include "ValueType.h"
#include <string>
#include <vector>

#include <any>
//might be an error because copying over iterators
namespace jbon {
	class Object :
		public ObjectType,
		public ValueTypeContract<Object>
	{
	private:
		friend class ValueTypeContract<Object>;
		friend class ValueRegistration<Object>;		
		static bool isSameType(std::string_view str);
		static Object * create(std::string_view str);
		static std::pair<char, char> getSurroundingDelimeters();
		//everything has to be a pointer because thats what's used
	public:
		Object();
		Object(std::string_view input);

		//copying inorder to retain insertion order
		Object(const Object& other);
		Object& operator=(const Object& other);

		//serializable functions
		std::string serialize() const;
		std::string serializeValues() const;

		Value& operator[](std::string const& key) {
			auto it = fields.find(key);
			if (it == fields.end()) {
				std::cerr << "Invalid Key Search:\"" << key << "\"" << std::endl;
				throw "Invalid Key Search";
			}
			return it->second;
		}

		template <typename T>
		void emplace(std::string const& key, T const& value) {
			push_back(key, value);
		}
		void erase(std::string key);
		friend std::ostream& operator<<(std::ostream& stream, const Object& object);
	

		static Object make(std::initializer_list<std::pair<std::string, Value>>const& values) {
			Object result;
			result.insertOrder.reserve(values.size());
			for (auto& value : values) {
				result.push_back(value.first, value.second);
			}
			return result;
		}
	};
}

