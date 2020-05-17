#pragma once
#include <string>
#include "object.h"
#include "SerializeRegistration.h"

namespace jbon {

	class SerializableType {
	protected:
		virtual SerializableType * createContract(std::optional<std::string> const& name, Object& object) = 0;
	public:
		virtual Object serialize() const = 0;
	};

	template <typename T>
	class Serializable
		: private SerializableType
	{
	private:
		static inline SerializeRegistration<T> registered;
		//never used
		SerializableType * createContract(std::optional<std::string> const& name, Object& object) {
			return T::create(name, object);
		}
	};
}

