#include "pch.h"
#include "Parser.h"
#include <iostream>
#include "StringFunctions.h"
#include <type_traits>
#include "object.h"
#include <optional>
//typedef std::variant<int, float, bool, std::string, Object> value_t;

namespace jbon {
	ClassCollection parse(std::string data) {
		auto const& serializedClasses = *Serialize_Registrants::getRegistrations();
		ClassCollection collection;
		removeSpaces(data);
		std::vector<std::string_view> entries;
		std::vector<int> dataIndecies;
		jbonFileSplit(entries, data);

		for (int i = 0; i < entries.size(); i++) {
			std::string_view& data = entries.at(i);
			char lastChar = data[data.size() - 1];
			if (lastChar == '}') {
				//get the class name to see if its a serialized class
				std::string name = "";
				for (int i = 0; i < data.size(); i++) {
					if (data[i] == '{') break;
					name += data[i];
				}
				//determine if this class is a registered serializedClass
				auto it = serializedClasses.find(name);
				if (it != serializedClasses.end()) {
					//push serialized class
					std::unique_ptr<BasicClass> transferPtr(it->second(data));
					collection.push_back(transferPtr, true);
				}
				else {
					//otherwise make a generic class
					std::unique_ptr<BasicClass> transferPtr = std::make_unique<GenericClass>(data);
					collection.push_back(transferPtr, true);
				}
			}
			else if (lastChar == ')') {
				dataIndecies.push_back(i);
			}
		}

		for (const auto& index : dataIndecies) {
			std::string_view& data = entries[index];
			//gets class name to query
			std::string className = "";
			for (int i = 0; i < data.size(); i++) {
				if (data[i] == ':' || data[i] == '(') {
					break;
				}
				className += data[i];
			}

			collection[className].makeAndAddObject(data);
		}
		return collection;
	}
}