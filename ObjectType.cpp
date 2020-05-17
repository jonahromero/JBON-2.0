#include "pch.h"
#include "ObjectType.h"

namespace jbon {
	void ObjectType::push_back(std::string key, Value value)
	{
		auto it = this->fields.emplace(key, value);
		insertOrder.push_back(it.first);
	}
	bool ObjectType::contains(std::string key) const
	{
		return fields.count(key) > 0;
	}
	int ObjectType::size() const
	{
		return fields.size();
	}
}
