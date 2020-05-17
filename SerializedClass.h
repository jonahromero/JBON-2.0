#pragma once
#include "BasicClass.h"
#include <vector>
#include "object.h"
#include <map>

namespace jbon {

	template<typename T>
	class SerializedClass
		: public BasicClass
	{
	private:
		std::vector<T> objects;
		std::vector<std::string> optionalNames;
		void reserve(size_t size){
			objects.reserve(size);
		}
	public:
		SerializedClass() {}
		SerializedClass(std::string_view schema) : BasicClass(schema) {}

		//linear complexity for items in objects
		T& operator[](std::string const& key) {
			if (key == "") {
				throw "Invalid Search for \"\"";
			}
			int index = 0;
			for (int i = 0; i < objects.size(); i++) {
				if (optionalNames[i] == key) {
					index = i;
					break;
				}
			}
			return objects[index];
		}
		friend std::ostream& operator<<(std::ostream& stream, SerializedClass<T> const& passedClass) {
			return stream << passedClass.serialize();
		}
		std::vector<T>& getObjects() {
			return objects;
		}
		void push_back(T const& value) {
			optionalNames.push_back("");
			objects.push_back(value);
		}
		void makeAndAddObject(std::string_view parsed) {
			std::optional<std::string> name;
			std::vector<Value> values;
			//populate the these two vars ^
			parseObject(values, parsed, name);

			Object toPass;
			for (int i = 0; i < values.size(); i++) {
				toPass.emplace(keys[i], values[i]);
			}
			//if it has a name
			if (name.has_value()) {
				optionalNames.push_back(name.value());
			}
			else {
				optionalNames.push_back("");
			}

			//delete the allocated object passed
			T * createdType = T::create(name, toPass);
			objects.push_back(*createdType);
			delete createdType;
		}

		std::string serialize() const {
			std::string result = BasicClass::serialize();
			return result + serializeBody();
		}
		std::string serializeBody() const {
			std::string result = "";
			for (int i = 0; i < objects.size(); i++) {
				std::string body;
				std::string objectAsString = objects[i].serialize().serializeValues();
				//change to different delimeters
				objectAsString[0] = '(';
				objectAsString[objectAsString.size() - 1] = ')';
				result.reserve(className.size() + 1 + objectAsString.size());
				result.append(className);
				if (optionalNames[i] != "") {
					result.reserve(optionalNames[i].size());
					result.push_back(':');
					result.append(optionalNames[i]);
				}
				result.append(objectAsString);
				result.push_back('\n');
			}
			return result;
		}
	};
}
