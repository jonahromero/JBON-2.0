#pragma once
#include <unordered_map>
#include "Value.h"
#include <map>
namespace jbon {
	class ObjectType
	{
	protected:
		typedef std::map<std::string, Value> fields_t;
		//actual values, as they special
		fields_t fields;

		std::vector<fields_t::iterator> insertOrder;
		void push_back(std::string key, Value value); //properly adds pair to object

		ObjectType() {}; //only children are viable.
	public:
		virtual ~ObjectType() {}
		//the at methods are to edit an exsisting type at a key
		template <typename T>
		T& at(std::string key) {
			auto it = fields.find(key);
			if (it == fields.end()) {
				std::cerr << "Invalid Key Search:\"" << key << "\"" << std::endl;
				throw "Invalid Key Search";
			}
			return it->second.get<T>();
		}

		//the set methods can change the value type at a field to what they want
		template <typename T>
		void set(std::string key, T value) {
			auto it = fields.find(key);
			it->second = value;
		}
		bool contains(std::string key) const;
		int size() const;
	};
}
