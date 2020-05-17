
#include "pch.h"
#include "value.h"
#include <iostream>
#include "StringFunctions.h"
#include "ValueRegistration.h"
#include <charconv>
//types
#include "Array.h"
#include "Pair.h"
#include "object.h"
#include "ValueType.h"

namespace jbon {
	void Value::safelySetVariant(value_t const& value)
	{
		//delete whats in the variant
		if (std::holds_alternative<ValueType*>(this->var)) {
			(*refrences)--;
			refrences = nullptr;
			if ((*refrences) == 0) {
				delete std::get<ValueType*>(this->var);
			}
		}
		//okay, actually add the type
		if (std::holds_alternative<ValueType*>(value)) {
			refrences = new int(1);
		}
		var = value;
	}
	Value::Value()
	{
		//refrences = new int(1);
		/*refTag = "ValueTag " + std::to_string(count++);
		std::cout << "created Value " << refTag << std::endl;*/
	}
	Value::~Value()
	{
		if (refrences != nullptr) {
			(*refrences)--;
			/*std::cout << "deleted a refrence to " << refTag << ", Current Refrences to " << refTag << ": " << (unsigned int)*refrences << std::endl;*/
			if ((*refrences) == 0) {
				/*std::cout << "deleted Value " << refTag << std::endl;*/
				delete refrences;
				delete get<ValueType*>();
			}
		}
	}
	Value Value::parse(std::string_view str)
	{
		Value result;
		if (str.size() == 0) {
			std::cerr << "Cannot convert: \"" << str << "\" to value_t" << std::endl;
			result.var = static_cast<int>(0);
			return result;
		}
		if (isInt(str)) {
			static int mapping[] = {1,10,100,1000,10000,100000,1000000,10000000,100000000,1000000000};
			int length = str.size() - 1;
			int resultInt = 0;
			bool neg = false;
			if (str[0] == '-') { length - 1; neg = true; }
			for (int i = length; i >= 0; i--) {
				resultInt += ((str[i] - '0') * mapping[length - i]);
			}
			result.var = resultInt * (neg ? -1 : 1);
			return result;
			
			/*std::cout << str << std::endl;
			const char* begin = str.data();
			const char* end = begin + str.size();	
			long long value;
			std::from_chars(begin,end, value);
			result.var = static_cast<int>(value);
			return result;*/
		}
		else if (isFloat(str)) {
			const char* begin = str.data();
			const char* end = begin + str.size();
			float value;
			std::from_chars(begin, end, value);
			result.var = value;
			return result;
		}
		else if (isBool(str)) {
			//if its a bool, then itll either be "true" or "false"
			result.var = (str == "true");
			return result;
		}
		else if (isString(str)) {
			result.var = std::string{ str.substr(1, str.size() - 2) };
			return result;
		}
		for (auto& functors : *Registrants::getRegistrations()) {
			result.refrences = new int(1);
			//first checks if its that type of object
			if (functors.first(str)) {
				//second makes an object of that type
				result.var = functors.second(str);
				return result;
			}
		}
	}
	std::string Value::serialize() const
	{
			std::string result = std::visit([](auto&& arg) -> std::string {
			using T = std::decay_t<decltype(arg)>;
			//need to collapse all classes into one if statment
			if constexpr (std::is_same_v<T, int> || std::is_same_v<T, float>) {
				char buffer[32];
				auto [endPtr, err] = std::to_chars(buffer, buffer + 32, arg);
				return std::string(buffer, endPtr - buffer);
			}
			else if constexpr (std::is_same_v<T, std::string>) {
				std::string result = "\"";
				result.append(arg);
				result.push_back('\"');
				return result;
			}
			else if constexpr (std::is_same_v<T, ValueType*>) {
				return arg->serialize();
			}
			else if constexpr (std::is_same_v<T, bool>) {
				return arg ? "true" : "false";
			}
			else {
				return "";
			}
		}, var);
		return result;
	}
	Value::Value(const Value & other)
	{
		refrences = other.refrences;
		if (refrences != nullptr) {
			(*refrences)++;
		}
		/*this->refTag = other.refTag;
		std::cout << "created a refrence to " << refTag << " by copy constructor"<<std::endl;*/
		this->var = other.var;
	}
	Value & Value::operator=(const Value & other)
	{	
		//if its not a basic type, its a nullptr
		if (refrences != nullptr) {
			(*refrences)--;
			/*
			std::cout << "deleted a refrence to " << refTag << ", Current Refrences to "<< refTag <<": "<< (unsigned int)*refrences <<std::endl;*/
			if ((*refrences) == 0) {
				/*std::cout << "deleted Value " << refTag << std::endl;*/
				delete refrences;
				delete get<ValueType*>();
			}
		}
		/*
		this->refTag = other.refTag;
		std::cout << "created a refrence to " << refTag << " by = operator"<<std::endl;
		*/
		this->var = other.var;
		this->refrences = other.refrences;
		if (refrences != nullptr) {
			(*refrences)++;
		}
		return *this;
	}
	Value::value_t const& Value::variant() const
	{
		return var;
	}
}