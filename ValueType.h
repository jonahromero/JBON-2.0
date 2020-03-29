#pragma once
#include <string>
#include "ValueRegistration.h"

/*
add this for private, otherwise you can just put it in public
private:
		friend class ValueTypeContract<Object>;
		friend class ValueRegistration<Object>;
		static bool isSameType(std::string str);
		static Object * create(std::string str);
		static std::pair<char, char> getSurroundingDelimeters();
*/
namespace jbon {
	class ValueType {
	private:
		friend class Value;
		//these functions wont be used
		virtual bool virtualIsSameType(std::string str) = 0;
		virtual ValueType * virtualCreate(std::string str) = 0;
		virtual std::pair<char,char> virtualGetSurroundingDelimeters() = 0;
		//needs to eventually be implemented
		virtual std::string serialize() = 0;
	public:
		ValueType() {}
		virtual ~ValueType() {}
	};

	template <typename T>
	class ValueTypeContract :
		public ValueType
	{
	private:
		//register the class
		static inline ValueRegistration<T> registered;
		virtual std::string serialize() = 0;
		//these wont ever actually be used, but they require the child to implement the static methods
		bool virtualIsSameType(std::string str) {
			std::cout << "hi";

			return T::isSameType(str);
		}
		//should return malloced pointer of child
		ValueType * virtualCreate(std::string str) {
			return T::create(str);
		}
		std::pair<char, char> virtualGetSurroundingDelimeters() {
			return T::getSurroundingDelimeters();
		}
	public:
		ValueTypeContract() {}
		virtual ~ValueTypeContract() {}
		
		//user still needs to implement serialize()
	};

	template<typename D, typename B>
	class isDerived {
		class No {};
		//just to make sure yes is different in size than no
		class Yes { No no[3]; };

		static Yes Test(B*);
		static No Test(...);
	public:
		enum {is = sizeof(Test(static_cast<D*>(0))) == sizeof(Yes) };
	};

}

