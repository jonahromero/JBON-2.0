#pragma once
#include <memory>
#include <string>
#include <variant>
#include "ValueType.h"
#include <type_traits>

namespace jbon {

	class ValueType;

	class Value {
		typedef std::variant<int, float, bool, std::string, ValueType*> value_t;
		value_t var;
		int * refrences = nullptr;
		//smart ptr debug
		//static inline int count = 1;
		//std::string refTag;
		
		Value(value_t var);
	public:
		Value();
		//serializing/parsing
		~Value();
		static Value parse(std::string asString);
		std::string serialize();
		//refrence counting
		Value(const Value& other);
		Value& operator=(const Value& other);
		//variant methods
	private:
		template <typename T, bool B>
		struct Getter {
			static T& _(value_t& var);
		};
		template <typename T>
		struct Getter<T, true> {
			static T& _(value_t& var) {
				return *dynamic_cast<T*>(std::get<ValueType*>(var));
			}
		};
		template <typename T>
		struct Getter<T, false>
		{
			static T& _(value_t& var) {
				return std::get<T>(var);
			}
		};
		//true if its derived
		template <typename T, bool N>
		struct Setter {
			static void _(T value, Value& var);
		};
		template <typename T>
		struct Setter<T, true> {
			static void _(T value, Value& var) {
				var = Value(new T(value));
			}
		};
		template <typename T>
		struct Setter<T, false>{
			static void _(T value, Value& var) {
				var = Value(value);
			}
		};
	public:
		template <typename T>
		T& get() { return Getter<T, isDerived<T, ValueType>::is>::_(this->var); }
		template <typename T>
		static Value make(T& value) {
			Value result;
			Setter<T, isDerived<T, ValueType>::is>::_(value, result);
			return result;
		}

		template <typename T>
		bool holds() { return std::holds_alternative<T>(var); }
		//should only be used for std::visit
		value_t& variant();
	};
}