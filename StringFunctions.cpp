#include "pch.h"
#include "StringFunctions.h"
#include <iostream>
#include "ValueRegistration.h"
#include <atomic>
#include <thread>
#include <mutex>

#define ESCAPE_CHAR '\\'
namespace jbon {
	void removeSpaces(std::string& str)
	{
		std::string newString = "";
		for (int i = 0; i < str.size(); i++) {
			char letter = str.at(i);
			if (letter == ' ' || letter == '\t' || letter == '\n' || letter == '\r') {
				continue;
			}
			newString.push_back(letter);
		}
		str = newString;
	}
	void indent(std::string& str, int spaces)
	{
		for (int i = 0; i < str.size(); i++) {
			if (i == 0 || str.at(i - 1) == '\n') {
				for (int j = 0; j < spaces; j++)
					str.insert(str.begin() + i, ' ');
			}
		}
	}

	auto splitTitleBody(std::string_view str, char bdel, char edel) -> std::pair<std::string_view, std::string_view>
	{
		std::pair<std::string_view, std::string_view> titleBodyResult;
		int firstDel = 0;
		for (int i = 0; i < str.length(); i++) {
			if (str[i] == bdel) {
				titleBodyResult.first = str.substr(0, i);
				firstDel = i;
				break;
			}
		}
		for (int i = str.length() - 1; i >= 0; i--) {
			if (str.at(i) == edel) {
				titleBodyResult.second = str.substr(firstDel, 1 + i - firstDel);
				break;
			}
		}
		return titleBodyResult;
	}
	void split(std::vector<std::string>& entries, std::string_view str, char delim) {
		int last = 0;
		//POSSIBLE OPTIMIZATION, SEARCH FOR ALL DELIMETERS AND RESERVE ON VECTOR
		for (int i = 0; i < str.size(); i++) {
			if (str[i] == delim) {
				entries.push_back(std::string{ str.substr(last, i - last) });
				last = i + 1; //it was at comma position
			}
		}
		entries.push_back(std::string{ str.substr(last, str.size() - last) });
	}
	void split(std::vector<std::string_view>& entries, std::string_view str, char delim)
	{
		int last = 0;
		//POSSIBLE OPTIMIZATION, SEARCH FOR ALL DELIMETERS AND RESERVE ON VECTOR
		for (int i = 0; i < str.size(); i++) {
			if (str[i] == delim) {
				entries.push_back(str.substr(last, i - last));
				last = i + 1; //it was at comma position
			}
		}
		entries.push_back(str.substr(last, str.size() - last));
	}
	void split(std::vector<std::string_view>& entries, std::string_view str, ExclusionDelimeters& exclude, char delim) {
		unsigned length = 0;
		int last = 0;
		int inExcluderBlock = 0;
		for (int i = 0; i < str.size(); i++) {
			const char c = str[i];

			ExclusionDelimeters::iterator it = exclude.find(c);
			if (it != exclude.end()) {
				if (it->first == c) {
					if (it->first == it->second) { 
						it->counter() = ~it->counter();
						if(it->counter() > 0)
							inExcluderBlock++;
						else
							inExcluderBlock--;
					} // handle first and second are equal edge-case
					else {
						it->counter()++;
						inExcluderBlock++;
					};
				}
				else if (it->second == c) {
					inExcluderBlock--;
					it->counter()--;
				}
			}
			if (c == delim && inExcluderBlock == 0) {
				entries.push_back(std::string_view{ str.data() + last, length });
				length = 0;
				last = i + 1;
			}
			else length++;

			if ((i == str.size() - 1) && inExcluderBlock == 0) {
				//just length because that length up there, will add it unfrotunatley
				//std::cout << "PUSHED A WORD" << std::endl;
				entries.push_back({ str.data() + last, length});
			}


			//std::cout << str << std::endl;
			//for (int j = 0; j < i; j++)
			//	std::cout << " ";
			//std::cout << "^" << std::endl;
			//std::cout << "In excluder block: " << (inExcluderBlock ? "true" : "false") << std::endl;
			//std::cout << "{} counter:" << exclude.find('}')->counter() << std::endl;
			//std::cout << "{} counter:" << exclude.find('{')->counter() << std::endl;
			//std::cout << "() counter:" << exclude.find('(')->counter() << std::endl;
			//std::cout << "() counter:" << exclude.find(')')->counter() << std::endl;
			//std::cout << "[] counter:" << exclude.find('[')->counter() << std::endl;
			//std::cout << "[] counter:" << exclude.find(']')->counter() << std::endl;
			//std::cout << std::endl;
		}

	}
	void jbonSplit(std::vector<std::string_view>& parsed, std::string_view str, char delims)
	{
		split(parsed, str, *Registrants::getDelimeters(), delims);
	}
	//void ApplyTojbonSplit(std::string_view str, std::function<void(std::string_view const&)> func)
	//{
	//	auto& exclude = *Registrants::getDelimeters();
	//	constexpr char delim = ',';
	//	int last = 0;
	//	unsigned length = 0;
	//	bool inExcluderBlock = false;
	//	//thread
	//	std::atomic<bool> done = false;
	//	std::atomic<bool> change = false;
	//	std::atomic<std::string_view> section;
	//	static std::thread apply([&section, &func, &done, &change]() {
	//		std::mutex m;
	//		while (!done) {
	//			if (change) {
	//				m.lock();
	//				change = false;
	//				std::cout << "new section" << std::endl;
	//				func(section);
	//				m.unlock();
	//			}
	//		}
	//	});
	//	
	//	for (int i = 0; i < str.size(); i++) {
	//		const char c = str[i];
	//		ExclusionDelimeters::iterator it = exclude.find(c);
	//		if (it != exclude.end()) {
	//			if (it->first == c) {
	//				if (it->first == it->second) it->counter() = ~it->counter(); // handle first and second are equal edge-case
	//				else it->counter()++;
	//			}
	//			else if (it->second == c) {
	//				it->counter()--;
	//			}
	//			if (it->counter() > 0) inExcluderBlock = true;
	//			else inExcluderBlock = false;
	//		}
	//		if (c == delim && !inExcluderBlock) { 
	//			change = true;
	//			section = { str.data() + last, length };
	//			
	//			//func(std::string_view((str.data() + last), length));
	//			length = 0;
	//			last = i + 1;
	//		}
	//		else length++;
	//		if ((i == str.size() - 1) && !inExcluderBlock) {
	//			change = true;
	//			section = {str.data() + last, length};
	//			//just length because that length up there, will add it unfrotunatley
	//		}
	//	}
	//	//done = true;
	//	apply.join();
	//}
	void jbonFileSplit(std::vector<std::string_view>& objects, std::string_view str)
	{
		auto& delimeters = *Registrants::getDelimeters();

		unsigned length = 0;
		int last = 0;
		int inExcluderBlock = 0;
		bool prevWasToken = false;
		for (int i = 0; i < str.size(); i++) {
			const char c = str[i];
			ExclusionDelimeters::iterator it = delimeters.find(c);
			if (it != delimeters.end()) {
				if (it->first == c) {
					if (it->first == it->second) {
						it->counter() = ~it->counter();
						if (it->counter() > 0)
							inExcluderBlock++;
						else
							inExcluderBlock--;
					} // handle first and second are equal edge-case
					else {
						it->counter()++;
						inExcluderBlock++;
					};
				}
				else if (it->second == c) {
					inExcluderBlock--;
					it->counter()--;
				}
			}

			
			if (prevWasToken && inExcluderBlock == 0) {
				objects.push_back({str.data() + last, length});
				last = i;
				length = 0;
			}
			//special case if its the end
			if ((i == str.size() - 1) && inExcluderBlock == 0) {
				objects.push_back({ str.data() + last, length + 1});
			}
			length++;
			if (!inExcluderBlock &&(c == '}' || c == ')')) {
				prevWasToken = true;
			}
			else prevWasToken = false;
		}
	}
	std::vector<std::string> splitClasses(std::string str)
	{
		int beginIndex = 0;
		std::vector<std::string> classes;
		for (int i = 0; i < str.size(); i++) {
			if (str.at(i) == '}') {
				classes.push_back(str.substr(beginIndex, 1 + i - beginIndex));
				beginIndex = i + 1;
			}
		}
		return classes;
	}
	std::vector<std::string> splitClassObjects(std::string str)
	{
		std::vector<std::string> classObjects;
		int beginIndex = 0;	//where we start for substr
		int bracketsOpen = 0; // see if were inside of two tokens
		bool stringOpen = false;
		//start at 1 because were doing i - 1, and nothing will happen at first index
		for (int i = 1; i < str.size(); i++) {
			//check for backslash because its an escape character
			if (str.at(i - 1) != ESCAPE_CHAR) {
				if (str.at(i) == '(' && !stringOpen) {
					bracketsOpen++;
				}
				else if (str.at(i) == ')' && !stringOpen) {
					bracketsOpen--;
				}
				//strings
				else if (str.at(i) == '\"') {
					stringOpen = !stringOpen;
				}
			}
			//after all opening logic, if nothings open at this point, and we're at the end,
			//then we can conclude that we've reached the end of an objectClass
			if (str.at(i) == ')' && bracketsOpen == 0) {
				classObjects.push_back(str.substr(beginIndex, 1 + i - beginIndex));
				beginIndex = i + 1;
			}
		}
		return classObjects;
	}

	/**/
	bool isNumeric(std::string_view str, bool& hasPeriods)
	{
		int periodCount = 0;
		int length = str.length();
		hasPeriods = false;
		int i = 0;
		if (str[0] == '-') {
			if (length == 1) {
				return false;
			}
			i = 1;
		}
		for (; i < length; i++) {
			char c = str[i];
			if (c == '.') {
				hasPeriods = true;
				periodCount++;
				if (periodCount > 1) return false;
			}
			else if (c < '0' || c > '9') return false;
		}
		return true;
	}
	bool isNumeric(std::string_view str)
	{
		int periodCount = 0;
		int length = str.length();
		int i = 0;
		if (str[0] == '-') {
			if (length == 1) {
				return false;
			}
			i = 1;
		}
		for (; i < length; i++) {
			char c = str[i];
			if (c == '.') {
				periodCount++;
				if (periodCount > 1) return false;
			}
			else if (c < '0' || c > '9') return false;
		}
		return true;
	}
	bool isFloat(std::string_view str)
	{
		return isNumeric(str);
	}
	bool isInt(std::string_view str)
	{
		bool hasPeriods; //isNumeric inits this
		return isNumeric(str, hasPeriods) && !hasPeriods;
	}
	bool isBool(std::string_view str)
	{
		return (str == "true" || str == "false");
	}
	bool isString(std::string_view str)
	{
		int unescapedQuotes = 0;
		for (int i = 0; i < str.size(); i++) {
			if ((i == 0 && str[i] == '\"')
				|| (i > 0 && str[i] == '\"' && str[i - 1] != '\\')) {
				unescapedQuotes++;
			}
		}
		//if the only two unescaped quotes are the first and last characters of string
		return str[0] == '\"' && str[str.size() - 1] == '\"' && unescapedQuotes == 2;
	}

	ExclusionDelimeters::ExclusionDelimeters()
	{
	}
	ExclusionDelimeters::ExclusionDelimeters(std::initializer_list<CharPair> delimeters)
	{
		for (auto& delim : delimeters)
			insert(delim);
	}
	void ExclusionDelimeters::insert(CharPair const & value)
	{
		if (value.first == value.second) {
			lookupTable[(int)value.first] = value;
			lookupTable[(int)value.first].counterPtr = &counters[(int)value.first];
		}
		else {
			lookupTable[(int)value.first] = value;
			lookupTable[(int)value.second] = value;
			//ptr
			lookupTable[(int)value.first].counterPtr = &counters[(int)value.first];
			lookupTable[(int)value.second].counterPtr = &counters[(int)value.first];
		}
	}
	void ExclusionDelimeters::insert(char first, char second)
	{
		if (first == second) {
			lookupTable[(int)first] = { first,second };
			lookupTable[(int)first].counterPtr = &counters[(int)first];
		}
		else {
			lookupTable[(int)first] = { first,second };
			lookupTable[(int)second] = { first,second };

			lookupTable[(int)first].counterPtr = &counters[(int)first];
			lookupTable[(int)second].counterPtr = &counters[(int)first];
		}
	}
	ExclusionDelimeters::iterator ExclusionDelimeters::find(const char search)
	{
		if (lookupTable[(int)search].first != 0) {
			return lookupTable.begin() + (int)search;
		}
		else
			return this->end();
	}
	ExclusionDelimeters::const_iterator ExclusionDelimeters::find(const char search) const
	{
		if (lookupTable[(int)search].first != 0) {
			return lookupTable.begin() + (int)search;
		}
		else
			return this->end();
	}
}