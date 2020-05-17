#pragma once
#include <string>
#include <vector>
#include <functional>
#include <array>
#include <string_view>

namespace jbon {

	class ExclusionDelimeters {

		struct mUint {
			unsigned int value = 0;
		};
		struct CharPair {
		private:
			friend class ExclusionDelimeters;
			mUint * counterPtr;
		public:
			CharPair() {}
			CharPair(const char first, const char second)
				: first(first), second(second) {}
			char first = 0, second = 0;
			unsigned int& counter() { return counterPtr->value; }
		};
		std::array<CharPair, 127> lookupTable;
		std::array<mUint, 127> counters;
	public:
		typedef std::array<CharPair, 127>::iterator iterator;
		typedef std::array<CharPair, 127>::const_iterator const_iterator;
		iterator begin() { return lookupTable.begin(); }
		const_iterator begin()const { return lookupTable.begin(); }
		iterator end() { return lookupTable.end(); }
		const_iterator end()const { return lookupTable.end(); }

		ExclusionDelimeters();
		ExclusionDelimeters(std::initializer_list<CharPair> delimeters);
		void insert(CharPair const& value);
		void insert(char first, char second);
		iterator find(const char search);
		const_iterator find(const char search) const;
	};

	void removeSpaces(std::string& str);
	void indent(std::string& str, int spaces = 0);
	//begins at the begining looking for bdel and then starts from back looking for edel
	auto splitTitleBody(std::string_view str, char bdel, char edel) -> std::pair<std::string_view, std::string_view>;
	void split(std::vector<std::string>& parsed, std::string_view str, char delim = '\n');
	void split(std::vector<std::string_view>& parsed, std::string_view str, char delim = '\n');
	//will split by a delimeter, but will not look for delimeters within any pair of chars
	void split(std::vector<std::string_view>& parsed,std::string_view str, ExclusionDelimeters& exclude, char delim = '\n');
	//any currently used delimeters will be taken into account
	void ApplyTojbonSplit(std::string_view str, std::function<void(std::string_view const&)> func) = delete;
	void jbonSplit(std::vector<std::string_view>& parsed,std::string_view str, char delims = ',');
	void jbonFileSplit(std::vector<std::string_view>& objects, std::string_view str);
	std::vector<std::string> splitClasses(std::string str);
	std::vector<std::string> splitClassObjects(std::string str);

	//functions testing ones. dont use for now.

	//checking as strings
	bool isNumeric(std::string_view str);
	bool isNumeric(std::string_view str, bool& hasPeriods);
	bool isFloat(std::string_view str);
	bool isInt(std::string_view str);
	bool isBool(std::string_view str);
	bool isString(std::string_view str);
}