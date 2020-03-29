#pragma once
#include "value.h"
#include <vector>
#include "ValueType.h"
#include "ValueRegistration.h"

namespace jbon {
	class Array : 
		public ValueTypeContract<Array>
	{
		std::vector<Value> values;
		friend class ValueTypeContract<Array>;
		friend class ValueRegistration<Array>;
		static Array * create(std::string str);
		static bool isSameType(std::string str);
		static std::pair<char, char> getSurroundingDelimeters();
	public:
		
		Array(std::string str);
		Array();
		~Array();
		std::string serialize();
	};
}
