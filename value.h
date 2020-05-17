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

		void safelySetVariant(value_t const& value);
		//smart ptr debug
	/*	static inline int count = 1;
		std::string refTag;*/
		
	public:
		template <typename T>
		Value(T value) {
			Setter<T, isDerived<T, ValueType>::is>::_(value, *this);
		}
		Value();
		//serializing/parsing
		~Value();
		//needs to be passed something with no spaces
		static Value parse(std::string_view asString);
		//static Value parse(std::string_view && asString);
		std::string serialize() const;
		//refrence counting
		Value(const Value& other);
		Value& operator=(const Value& other);
		//when u equate a Type to a value, theres some stuff you can skip.
		template<typename T>
		Value& operator=(const T& other) {
			this->safelySetVariant(other);
			return *this;
		}
		//variant methods
		template <typename T>
		T& get() { return Getter<T, isDerived<T, ValueType>::is>::_(this->var); }

		template <typename T>
		bool is() const { return Checker<T, isDerived<ValueType, T>::is>::_(*this); }

		//should only be used for std::visit
		value_t const& variant() const;
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
				var.safelySetVariant(new T(value));
			}
		};
		template <typename T>
		struct Setter<T, false>{
			static void _(T value, Value& var) {
				var.safelySetVariant(value);
			}
		};
		//might wanna get rid of this.
		template <>
		struct Setter<Value, false> {
			static void _(Value value, Value& var) {
				var = value;
			}
		};
		//two extra specializations, because compiler assumes dumb ass shit
		template <>
		struct Setter<const char*, false> {
			static void _(const char * value, Value& var) {
				var.safelySetVariant(std::string(value));
			}
		};
		template <>
		struct Setter<double, false> {
			static void _(double value, Value& var) {
				var.safelySetVariant((float)value);
			}
		};
		template <typename T, bool N>
		struct Checker {
			static bool _(Value const& value);
		};
		template <typename T>
		struct Checker<T,true> {
			static bool _(Value const& value) {
				T* typePtr = dynamic_cast<T*>(std::get<ValueType*>(value.var));
				//if its a nullptr it failed the cast
				return (typePtr != nullptr);
			}
		};
		template <typename T>
		struct Checker<T, false> {
			static bool _(Value const& value) {
				return std::holds_alternative<T>(value.var);
			}
		};
	};

}