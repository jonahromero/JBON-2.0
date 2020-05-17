#include "pch.h"
#include "ObjectCollection.h"
namespace jbon {
	ObjectCollection::ObjectCollection() {}
	ObjectCollection::iterator ObjectCollection::find(std::string name)
	{
		return std::find_if(objects.begin(), objects.end(),
			[=](const ClassObject& object) -> bool {
			if (object.name() != "") //if the object has a name
				return (object.name() == name);
			else
				return false;
		}
		);
	}
	ClassObject & jbon::ObjectCollection::operator[](std::string const& name)
	{
		auto it = find(name);
		if (it == objects.end()) {
			throw std::runtime_error("Invalid Object Name: " + name);
		}
		else {
			return *it;
		}
	}
}