#pragma once
#include "ClassObject.h"
#include <vector>
#include <algorithm>

namespace jbon {
	class ObjectCollection
	{
		//the only thing that should be able to touch these things
		//is a generic class, because it makes objecCollections, so 
		//itll be a friend ):
		friend class GenericClass;
		std::vector<ClassObject> objects;
		ObjectCollection();
	public:
		//iterators and containers
		typedef std::vector<ClassObject>::iterator iterator;
		typedef std::vector<ClassObject>::const_iterator const_iterator;

		iterator begin() { return objects.begin(); }
		iterator end() { return objects.end(); }
		const_iterator begin() const { return objects.begin(); }
		const_iterator end() const { return objects.end(); }

		iterator find(std::string name); //finds a certain object based on name
		void erase(iterator it) { objects.erase(it); }
		int size() { return objects.size(); }
		bool empty() { return objects.empty(); }

		ClassObject& operator[](std::string const& name);
	};
}

