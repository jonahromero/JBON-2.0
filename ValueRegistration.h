#pragma once
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <iostream>
#include <functional>	
namespace jbon {
	class ValueType;
	//struct that contains all the information about a current registered classes
	struct Registrants {
		typedef std::vector<std::pair<std::function<bool(std::string)>,
			std::function<ValueType*(std::string)>>> vec_t;

		typedef std::vector<std::pair<char, char>> delimeters_t;
		static vec_t * getRegistrations() {
			if (!registrations) { registrations = new vec_t(); }
			return registrations;
		}
		static delimeters_t * getDelimeters() {
			if (!delimeters) { delimeters = new delimeters_t(); }
			return delimeters;
		}
	private:
		//never deleted because itll get chopped randomly with smart ptrs
		static inline vec_t * registrations;
		static inline delimeters_t * delimeters;
	};
	//registers the class by calling their methods
	template <typename T>
	auto createFunction(T func) -> std::function<ValueType*(std::string)> {
		return [=](std::string str)->ValueType* {
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
			getDelimeters()->push_back(T::getSurroundingDelimeters());
		}
		~ValueRegistration() {}
	};
}
