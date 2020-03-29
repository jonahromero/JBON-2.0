#pragma once
#include <string>
#include <vector>
#include<unordered_map>
#include "GenericClass.h"
#include "value.h"

namespace jbon {
	
	class Parser
	{
	protected:
		//only reason for map is to get instant access to class based on creating class object
		std::unordered_map<std::string, GenericClass> classes;
	public:
		Parser(std::string headers, std::string data);
		GenericClass& getClass(std::string name);
		std::vector<GenericClass> getClasses();
		~Parser();
		std::string to_string();
	};

}