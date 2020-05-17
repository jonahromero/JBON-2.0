#pragma once
#include "ValueType.h"
#include "value.h"

namespace jbon {
	class Pair : 
		public ValueTypeContract<Pair>
	{
		friend class ValueTypeContract<Pair>;
		friend class ValueRegistration<Pair>;
		static Pair * create(std::string_view str);
		static bool isSameType(std::string_view str);
		static std::pair<char, char> getSurroundingDelimeters();

		Value first, second;
	public:
		template <typename T, typename U>
		static Pair make(T first, U second) {
			Pair result;
			result.first = first;
			result.second = second;
			return result;
		}
		std::string serialize() const;

		Pair(std::string_view str);
		Pair();
		~Pair();
		template<typename T>
		T& key() {
			return first.get<T>();
		}
		template<typename T>
		T& value() {
			return second.get<T>();
		}
		//setting
		template<typename T>
		void setKey(const T& key) {
			return first = key;
		}
		template<typename T>
		void setValue(const T& value) {
			return second = value;
		}
		void swap();
		
	};
}
