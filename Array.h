#pragma once
#include "value.h"
#include <vector>
#include "ValueType.h"
#include "ValueRegistration.h"
#include <type_traits>
#include "Parser.h"

namespace jbon {
	class Array :
		public ValueTypeContract<Array>
	{
	private:
		friend class ValueTypeContract<Array>;
		friend class ValueRegistration<Array>;
		std::vector<Value> values;
		static Array * create(std::string_view str);
		static bool isSameType(std::string_view str);
		static std::pair<char, char> getSurroundingDelimeters();
	public:
		typedef typename std::vector<Value>::iterator iterator;
		typedef typename std::vector<Value>::const_iterator const_iterator;
		iterator begin() { return values.begin(); }
		iterator end() { return values.end(); }
		const_iterator begin() const { return values.begin(); }
		const_iterator end() const { return values.end(); }

		void erase(iterator it) { values.erase(it); }
		int size() { return values.size(); }
		bool empty() { return values.empty(); }
		template <typename T>
		T& at(const int index) { return values[index]; }
		template <typename T>
		std::vector<T> getVector() {
			std::vector<T> result;
			for (auto& value : values) {
				result.push_back(value.get<T>());
			}
			return result;
		}
		Array(std::string_view str);
		Array() {}
		~Array() {}

		std::string serialize() const;
		template<typename T>
		void push_back(const T& value) {
			values.push_back(value);
		}
		template<typename ...args>
		static Array make(args... values) {
			Array result;
			result.template_push_back(values);
			return result;
		}
	private:
		template<typename T, typename ...args>
		void template_push_back(T value, args... values) {
			values.push_back(value);
			template_push_back(values...);
		}
		template<typename T>
		void template_push_back(T value) {
			values.push_back(value);
		}
	};
}
