#pragma once
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <functional>
#include "StringFunctions.h"

namespace jbon {
	class ValueType;
	//struct that contains all the information about a current registered classes
	struct Registrants {
		typedef std::vector<std::pair<std::function<bool(std::string_view)>,
			std::function<ValueType*(std::string_view)>>> vec_t;

		typedef ExclusionDelimeters delimeters_t;
		static vec_t * getRegistrations() {
			if (!registrations) { registrations = new vec_t(); }
			return registrations;
		}
		static delimeters_t * getDelimeters() {
			if (!delimeters) { 
				delimeters = new delimeters_t(); 
				delimeters->insert('\"', '\"');
				delimeters->insert('(', ')');
			}
			return delimeters;
		}
	private:
		//never deleted because itll get chopped randomly with smart ptrs
		static inline vec_t * registrations;
		static inline delimeters_t * delimeters ;
	};
	//registers the class by calling their methods
	template <typename T>
	auto createFunction(T func) -> std::function<ValueType*(std::string_view)> {
		return [=](std::string_view str)->ValueType* {
			return (ValueType*)func(str);
		};
	}
	template <typename T>
	class ValueRegistration :
		public Registrants
	{
	public:
		ValueRegistration() {
			auto pair = std::make_pair(&T::isSameType, createFunction(T::create));
			getRegistrations()->push_back(pair);
			auto delimPair = T::getSurroundingDelimeters();
			getDelimeters()->insert(delimPair.first, delimPair.second);
		}
	};
}
