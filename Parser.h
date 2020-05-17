#pragma once
#include <string>
#include <vector>
#include<unordered_map>
#include "GenericClass.h"
#include "value.h"
#include "StringFunctions.h"
#include "ValueType.h"
#include "ValueRegistration.h"
#include "ClassCollection.h"

namespace {
	template<typename T>
	T primParse(std::string str);
	template<>
	int primParse(std::string str) { return std::stoi(str); }
	template<>
	float primParse(std::string str) { return std::stof(str); }
	template<>
	bool primParse(std::string str) { return str == "true"; }
	template<>
	std::string primParse(std::string str) { return (str.substr(1, str.size() - 2)); }

	template<typename T, bool N>
	struct parser {
		static T _(std::string str);
	};
	template<typename T>
	struct parser<T, true> {
		static T _(std::string str) {
			jbon::removeSpaces(str);
			for (auto& functors : *Registrants::getRegistrations()) {
				//first checks if its that type of object
				if (functors.first(str)) {
					//second makes an object of that type
					T* ptr = functors.second(str);
					T value = *ptr;
					delete ptr;
					return value;
				}
			}
		}
	};
	template<typename T>
	struct parser<T, false> {
		static T _(std::string str) {
			return primParse<T>(str);
		}
	};
}
namespace jbon {
	ClassCollection parse(std::string data);
	template <typename T>
	T parseString(std::string key) {
		return parser<T, isDerived<T, ValueType>::is>::_(key);
	}
}