#pragma once
#include <string>
#include <vector>

namespace jbon {
	void removeSpaces(std::string& str);
	//begins at the begining looking for bdel and then starts from back looking for edel
	void splitTitleBody(std::string str, std::string& title, std::string& body, char bdel, char edel);
	std::vector<std::string> split(std::string str, char delim = '\n');
	//will split by a delimeter, but will not look for delimeters within any pair of chars
	std::vector<std::string> split(std::string str, std::vector<std::pair<char, char>> exclude, char delim = '\n');
	//any currently used delimeters will be taken into account
	std::vector<std::string> jbonSplit(std::string str, char delims = ',');
	std::vector<std::string> splitClasses(std::string str);
	std::vector<std::string> splitClassObjects(std::string str);
	void indent(std::string& str, int spaces = 0);
	//checking as strings
	bool isFloat(std::string str);
	bool isInt(std::string str);
	bool isBool(std::string str);
	bool isString(std::string str);
	bool isObject(std::string str);
	bool isArray(std::string str);
	bool isPair(std::string str);
}