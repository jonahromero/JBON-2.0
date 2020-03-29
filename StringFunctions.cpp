#include "pch.h"
#include "StringFunctions.h"
#include <iostream>
#include "ValueRegistration.h"
#define ESCAPE_CHAR '\\'
namespace jbon {
	void removeSpaces(std::string & str)
	{
		for (int i = 0; i < str.size(); i++) {
			char letter = str.at(i);
			if (letter == ' ' || letter == '\t' || letter == '\n') {
				str.erase(str.begin() + i);
				i--; // whole string was moved back one, so we need to back iteration up by one
			}
		}
	}
	void splitTitleBody(std::string str, std::string & title, std::string & body, char bdel, char edel)
	{
		int firstDel = 0;
		for (int i = 0; i < str.length(); i++) {
			if (str.at(i) == bdel) {
				title = str.substr(0, i);
				firstDel = i;
				break;
			}
		}
		for (int i = str.length() - 1; i >= 0; i--) {
			if (str.at(i) == edel) {
				body = str.substr(firstDel, 1 + i - firstDel);
				return;
			}
		}
	}
	std::vector<std::string> split(std::string str, char delim)
	{
		std::vector<std::string> entries;
		int last = 0;
		str += std::string(1, delim); // adds a delimeter to the end, so you include last part too
		for (int i = 0; i < str.size(); i++) {
			if (str.at(i) == delim) {
				entries.push_back(str.substr(last, i - last));
				last = i + 1; //it was at comma position
			}
		}
		//no comma at the end
		return entries;
	}
	std::vector<std::string> split(std::string str, std::vector<std::pair<char, char>> exclude, char delim)
	{
		std::vector<std::string> entries;
		std::vector<int> counters; //corresponding exclude
		str += std::string(1, delim); // adds a delimeter to the end, so you include last part too
		//initialize counters
		for (int i = 0; i < exclude.size(); i++) {
			counters.push_back(0);
		}
		int last = 0;
		for (int i = 0; i < str.size(); i++) {
			bool open = true; //if any delimeters are open
			//if any delimeters are used push up the counters, if theyre 0 nothing is open
			for (int j = 0; j < exclude.size(); j++) {
				//if theres an escape character ignore exclusion
				if (i > 0 && str.at(i - 1) == ESCAPE_CHAR) { 
					break;
				}
				if (exclude.at(j).first != exclude.at(j).second) {
					//increment or de-increment depending on if they're first or second token
					if (str.at(i) == exclude.at(j).first) {
						counters.at(j)++;
					}
					else if (str.at(i) == exclude.at(j).second) {
						counters.at(j)--;
					}
				}
				//otherwise if the pair of delimeters are the same
				else {
					if (str.at(i) == exclude.at(j).first && counters.at(j) < 1) {
						counters.at(j)++;
					}
					else if (str.at(i) == exclude.at(j).second) {
						counters.at(j)--;
					}
					
				}
				//if any of the counters arent closed (they're value is 0) then itll say its not open
				if (counters.at(j) != 0) {
					open = false;
				}
			}
			if (str.at(i) == delim && open) {
				entries.push_back(str.substr(last, i - last));
				last = i + 1;
			}
		}
		return entries;
	}
	std::vector<std::string> jbonSplit(std::string str, char delims)
	{
		std::vector<std::pair<char, char>> delimeters = *Registrants::getDelimeters();
		delimeters.emplace_back('\"', '\"');
		return split(str, delimeters, delims);
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

	void indent(std::string& str, int spaces)
	{
		for (int i = 0; i < str.size(); i++) {
			if (i == 0 || str.at(i - 1) == '\n') {
				for (int j = 0; j < spaces; j++)
					str.insert(str.begin() + i, ' ');
			}
		}
	}
	bool isFloat(std::string str)
	{
		int numOfNegatives = 0,
			numOfPeriods = 0;
		for (char& letter : str) {
			if (letter == '.') {
				numOfPeriods++;
			}
			else if (letter == '-') {
				numOfNegatives++;
			}
			else if (letter < '0' || letter > '9') {
				return false;
			}
		}
		if (numOfNegatives > 1 || numOfPeriods > 1
			|| (numOfNegatives == 1 && str.at(0) != '-')) {
			return false;
		}
		return true;
	}
	bool isInt(std::string str)
	{
		for (char& letter : str) {
			if (letter == '.') {
				return false;
			}
		}
		//at this point if it passes for being a float, and it has no periods its an int 
		return isFloat(str);
	}
	bool isBool(std::string str)
	{
		if (str == "true" || str == "false") {
			return true;
		}
		return false;
	}
	bool isString(std::string str)
	{
		int unescapedQuotes = 0;
		for (int i = 0; i < str.size(); i++) {
			if ((i == 0 && str.at(i) == '\"')
				|| (i > 0 && str.at(i) == '\"' && str.at(i - 1) != '\\')) {
				unescapedQuotes++;
			}
		}
		//if the only two unescaped quotes are the first and last characters of string
		return str.at(0) == '\"' && str.at(str.size() - 1) == '\"' && unescapedQuotes == 2;
	}
	bool isObject(std::string str)
	{
		//if it begins with { and ends with }, then its an object
		return str.at(0) == '\{' && str.at(str.size() - 1) == '\}';
	}
	bool isArray(std::string str)
	{
		return str.at(0) == '[' && str.at(str.size() - 1) == ']';
	}
	bool isPair(std::string str)
	{
		return str.at(0) == '<' && str.at(str.size() - 1) == '>';
	}
}