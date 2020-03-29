
#include "pch.h"
#include "value.h"
#include <iostream>
#include "StringFunctions.h"
#include "ValueRegistration.h"
//types
#include "Array.h"
#include "Pair.h"
#include "object.h"
#include "ValueType.h"

namespace jbon {
	Value::Value()
	{
		refrences = new int(1);
		//refTag = "ValueTag " + std::to_string(count++);
		//std::cout << "created Value " << refTag << std::endl;
	}
	Value::Value(value_t var)
	{
		refrences = new int(1);
		this->var = var;
		///*refTag = "ValueTag " + std::to_string(count++);
		///std::cout << "created Value "  << refTag << std::endl;*/
	}
	Value::~Value()
	{
		(*refrences)--;
		///std::cout << "deleted a refrence to " << refTag << ", Current Refrences to " << refTag << ": " << (unsigned int)*refrences << std::endl;
		if (*refrences == 0) {
			///*std::cout << "deleted Value " << refTag << std::endl;*/
			delete refrences;
			if (holds<ValueType*>()) {
				delete get<ValueType*>();
			}
		}
	}
	Value Value::parse(std::string str)
	{
		Value result;
		if (str.size() == 0) {
			std::cerr << "Cannot convert: " << str << " to value_t" << std::endl;
			result.var = static_cast<int>(0);
		}
		if (isInt(str)) {
			result.var = std::stoi(str);
		}
		else if (isFloat(str)) {
			result.var = std::stof(str);
		}
		else if (isBool(str)) {
			//if its a bool, then itll either be "true" or "false"
			result.var = (str == "true");
		}
		else if (isString(str)) {
			result.var = (str.substr(1, str.size() - 2));
		}
		for (auto& functors : *Registrants::getRegistrations()) {
			removeSpaces(str);
			//first checks if its that type of object
			if (functors.first(str)) {
				//second makes an object of that type
				result.var = functors.second(str);
			}
		}
		return result;
	}
	std::string Value::serialize()
	{

			std::string result = std::visit([](auto&& arg) -> std::string {
			using T = std::decay_t<decltype(arg)>;
			//need to collapse all classes into one if statment
			if constexpr (std::is_same_v<T, int>) {
				return std::to_string(arg);
			}
			else if constexpr (std::is_same_v<T, float>) {
				return std::to_string(arg);
			}
			else if constexpr (std::is_same_v<T, std::string>) {
				return "\"" + arg + "\"";
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
		(*refrences)++;
		///*this->refTag = other.refTag;
		///std::cout << "created a refrence to " << refTag << " by copy constructor"<<std::endl;*/
		this->var = other.var;
	}
	Value & Value::operator=(const Value & other)
	{	
		(*refrences)--;
		///*std::cout << "deleted a refrence to " << refTag << ", Current Refrences to "<< refTag <<": "<< (unsigned int)*refrences <<std::endl;*/
		if (*refrences == 0) {
			///*std::cout << "deleted Value " << refTag << std::endl;*/
			delete refrences;
			if (holds<ValueType*>()) {
				delete get<ValueType*>();
			}
		}
		///this->refTag = other.refTag;
		///*std::cout << "created a refrence to " << refTag << " by = operator"<<std::endl;*/

		this->var = other.var;
		this->refrences = other.refrences;
		(*refrences)++;
		return *this;
	}
	Value::value_t& Value::variant()
	{
		return var;
	}
}