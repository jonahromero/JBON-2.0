#pragma once
#include "Parser.h"
#include "ClassCollection.h"

namespace jbon {
	class File
	{
	private:
		std::string filepath;
		ClassCollection classCollection;
		bool isJbonFile(const std::string& filepath) const;
	public:
		ClassCollection& getClasses();
		File(std::string filepath);
		File();
		~File() {}
		void include(std::string const& filepath);
		void includeString(std::string text);
		void load(std::string filepath);
		void save();
		void close();
	};
}
