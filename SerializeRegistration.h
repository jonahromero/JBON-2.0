#pragma once
#include <vector>
#include <functional>
#include "object.h"
#include "SerializedClass.h"
namespace jbon {

	class SerializableType;
	struct Serialize_Registrants {
		//describes how a class, with name followed by how to make its SerializedClass
		typedef std::unordered_map<std::string, std::function<std::unique_ptr<BasicClass>(std::string_view)>> map_t;

		static map_t * getRegistrations() {
			if (!registrations) { registrations = new map_t(); }
			return registrations;
		}
	private:
		//never deleted because itll get chopped randomly with smart ptrs
		static inline map_t *registrations;
	};

	//called s because normal registration is for custom jbon types
	template <typename T>
	auto s_createFunction(T func) -> std::function<SerializableType*(jbon::Object)> {
		return [=](jbon::Object object)->SerializableType* {
			return (SerializableType*)func(object);
		};
	}
	template <typename T>
	std::string getTypeName() {
		//type is will give "class #T"
		std::string name = typeid(T).name();
		for (int i = 0; i < name.size(); i++) {
			if (name[i] == ' ') {
				name = name.substr(i + 1, name.size() - i);
				break;
			}
		}
		return name;
	}
	template <typename T>
	class SerializeRegistration :
		public Serialize_Registrants
	{
	public:
		SerializeRegistration() {
			getRegistrations()->emplace(getTypeName<T>(), [](std::string_view str)->std::unique_ptr<BasicClass> {
				return std::make_unique<SerializedClass<T>>(str);
			});
		}
	};
}