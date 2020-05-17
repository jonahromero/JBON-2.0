#pragma once
#include <unordered_map>
#include "GenericClass.h"
#include <string>
#include "SerializedClass.h"
#include "SerializeRegistration.h"
#include <map>

namespace jbon {
	class ClassCollection
	{
		//friend the file function that saves class collections
		struct classInfo {
			std::shared_ptr<BasicClass> ptr;
			bool saveInCereal;
			friend bool cmp(classInfo const& first, classInfo const& second) {
				return first.ptr->name() > second.ptr->name();
			}
		};
		std::map<std::string, classInfo> classes;
	public:	
		ClassCollection();
		//add something here for basicClasses
		void push_back(std::unique_ptr<BasicClass>&, bool const& saveInCereal); //add a new SerializedClass. this can be better
		//getters
		BasicClass& operator[](std::string key);
		template<typename T>
		SerializedClass<T>& getSerializedClasss() {
			std::string const typeName = getTypeName<T>();
			return *dynamic_cast<SerializedClass<T>*>(classes.at(typeName).ptr.get());
		}
		GenericClass& getGenericClass(std::string const& name) {
			return *dynamic_cast<GenericClass*>(classes.at(name).ptr.get());
		}
		int size();
		//strings
		std::string serialize();
		friend std::ostream& operator<< (std::ostream& stream, ClassCollection& collection);
	};
}
