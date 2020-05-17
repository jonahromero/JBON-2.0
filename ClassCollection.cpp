#include "pch.h"
#include "ClassCollection.h"
namespace jbon {
	jbon::ClassCollection::ClassCollection(){}

	void ClassCollection::push_back(std::unique_ptr<BasicClass>& serializedClass, bool const& saveInCereal)
	{
		std::string name = serializedClass->name();
		classInfo temp;
		temp.saveInCereal = saveInCereal;
		temp.ptr = std::move(serializedClass);
		classes.emplace(name, temp);
	}

	std::string ClassCollection::serialize()
	{
		std::string result = "";
		for (auto&[_, value] : classes) {
			if(value.saveInCereal)
				result.append(value.ptr->serialize());
			else
				result.append(value.ptr->serializeBody());
		}
		return result;
	}

	BasicClass& jbon::ClassCollection::operator[](std::string key)
	{
		auto it = classes.find(key);
		//if the key was not found
		if (it == classes.end()) {
			throw std::runtime_error("Unkown class: " + key);
		}
		else {
			return *classes[key].ptr;
		}
	}
	int ClassCollection::size()
	{
		return classes.size();
	}
	std::ostream & operator<<(std::ostream & stream, ClassCollection & collection)
	{
		stream << collection.serialize();
		return stream;
	}
}