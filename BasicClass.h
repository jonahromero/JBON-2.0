#pragma once
#include <string>
#include <vector>
#include "ObjectCollection.h"
#include "ClassObject.h"
#include <optional>

namespace jbon {

	void parseObject(std::vector<Value>& values, std::string_view str, std::optional<std::string>& name);

	class BasicClass
	{
	private:
	protected:
		std::string className;
		std::vector<std::string> keys; //all the keys from the schema

		BasicClass();
		BasicClass(std::string_view	schema);
	public:
		virtual ~BasicClass() {}
		//serialization/string functions
		virtual std::string serialize() const;
		virtual std::string serializeBody() const = 0;
		std::string name() const;

		//reserves space to add more objects
		virtual void reserve(size_t size) = 0;
		virtual void makeAndAddObject(std::string_view parse) = 0;
	};
}
