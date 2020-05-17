#include "pch.h"
#include "File.h"
#include <fstream>
#include <filesystem>
#include <thread>
#include <cstdio>
void unformattedExtractFile(const std::string& filepath, std::string& fileContents) {
	std::ifstream ifs(filepath.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

	std::ifstream::pos_type fileSize = ifs.tellg();
	if (fileSize < 0)
		return;
	ifs.seekg(0, std::ios::beg);
	fileContents.resize(fileSize);
	ifs.read(&fileContents.data()[0], fileSize);
}
void extractFile(const std::string& filepath, std::string& fileContents) {
	/*auto path = std::filesystem::path(filepath);
	auto size = std::filesystem::file_size(path);
	if (size == 0) { return; }

	fileContents.reserve(size);*/
	//15.9%
	//FILE * file = fopen(filepath.data(), "r");
	//char c = 0;
	//while ((c = fgetc(file)) != EOF) {
	//	if (c == ' ' || c == '\n' || c == '\t' || c == '\r')
	//		continue;
	//	fileContents.push_back(c);
	//} 
	//fclose(file);

	//8.75%
	/*std::ifstream file(filepath);
	fileContents.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
	jbon::removeSpaces(fileContents);*/

	//6.00%
	std::ifstream ifs(filepath.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

	std::ifstream::pos_type fileSize = ifs.tellg();
	if (fileSize < 0)
		return;
	ifs.seekg(0, std::ios::beg);
	fileContents.resize(fileSize);
	ifs.read(&fileContents.data()[0], fileSize);
	jbon::removeSpaces(fileContents);

	//10%
	/*std::FILE *fp = std::fopen(filepath.data(), "rb");
	std::fseek(fp, 0, SEEK_END);
	fileContents.resize(std::ftell(fp));
	std::rewind(fp);
	std::fread(&fileContents[0], 1, fileContents.size(), fp);
	std::fclose(fp);
	jbon::removeSpaces(fileContents);*/

	//10%
	/*std::ifstream in(filepath, std::ios::in);
	in.seekg(0, std::ios::end);
	fileContents.reserve(in.tellg());
	in.seekg(0, std::ios::beg);
	fileContents.assign((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	in.close();
	jbon::removeSpaces(fileContents);*/

	//26%
	//auto path = std::filesystem::path(filepath);
	//auto size = std::filesystem::file_size(path);
	//if (size == 0) { return; }

	//fileContents.reserve(size);
	//std::ifstream file(filepath);
	//std::istream_iterator<char> it(file);
	//std::istream_iterator<char> eos;
	//std::copy_if(it, eos, std::back_inserter(fileContents), [](const char c)->bool {
	//	return !(c == ' ' || c == '\n' || c == '\t' || c == '\r');
	//});
}

namespace jbon {
	bool File::isJbonFile(const std::string & filepath) const
	{
		return filepath.substr(filepath.size() - 5, 5) == ".jbon";
	}
	ClassCollection& File::getClasses()
	{
		return classCollection;
	}
	File::File(std::string filepath)
	{
		load(filepath);
	}
	File::File()
	{
	}
	void File::include(std::string const& filepath)
	{
		if (!isJbonFile(filepath)) return;
		std::string fileContents;
		unformattedExtractFile(filepath, fileContents);
		includeString(filepath); //remove spaces in there
	}
	void File::includeString(std::string fileContents)
	{
		removeSpaces(fileContents);
		auto const& serializedClasses = *Serialize_Registrants::getRegistrations();
		std::vector<std::string_view> objects;
		jbonFileSplit(objects, fileContents);

		for (auto& object : objects) {
			if (object[object.size() - 1] == '}') {
				//get the class name to see if its a serialized class
				std::string name = "";
				for (int i = 0; i < object.size(); i++) {
					if (object[i] == '{') break;
					name += object[i];
				}

				auto it = serializedClasses.find(name);
				if (it != serializedClasses.end()) {
					//push serialized class
					auto ptr = it->second(object);
					std::unique_ptr<BasicClass> transferPtr = std::move(ptr);
					classCollection.push_back(transferPtr, false);
				}
				else {
					//otherwise make a generic class
					std::unique_ptr<BasicClass> transferPtr = std::make_unique<GenericClass>(object);
					classCollection.push_back(transferPtr, false);
				}
			}
		}
	}
	void File::load(std::string filepath)
	{
		if (!isJbonFile(filepath)) {
			std::cerr << "JBON: Invalid file format. change extension to .jbon" << std::endl;
			return;
		}
		auto const& serializedClasses = *Serialize_Registrants::getRegistrations();
		this->filepath = filepath;
		std::string fileContents;
		extractFile(filepath, fileContents);
		std::vector<std::string_view> objects;
		std::vector<int> constructorIndecies;
		jbonFileSplit(objects, fileContents);
		//if they loaded an empty file, do nothing
		if (fileContents.size() == 0) {
			return;
		}
		////DEBUG
		//for (auto& object : objects) {
		//	std::cout << object << std::endl;
		//}

		for (int i = 0; i < objects.size(); i++) {
			std::string_view& data = objects[i];
			if (data[data.size() - 1] == '}') {
				std::string name = "";
				for (int i = 0; i < data.size(); i++) {
					if (data[i] == '{') break;
					name += data[i];
				}

				auto it = serializedClasses.find(name);
				if (it != serializedClasses.end()) {
					//push serialized class
					std::unique_ptr<BasicClass> transferPtr(it->second(data));
					classCollection.push_back(transferPtr,true);
				}
				else {
					//otherwise make a generic class
					std::unique_ptr<BasicClass> transferPtr = std::make_unique<GenericClass>(data);
					classCollection.push_back(transferPtr, true);
				}
			}
			else if (data[data.size() - 1] == ')') {
				constructorIndecies.push_back(i);
			}
		}
		for (auto& index : constructorIndecies) {
			std::string className = "";
			for (const auto& letter : objects[index]) {
				if (letter == '(' || letter == ':') break;	
				className += letter;
			}
			//add object with the data
			classCollection[className].makeAndAddObject(objects[index]);
		}
	}
	void File::save()
	{	
		std::ofstream file(filepath, std::ios::trunc);
		if (file.is_open()) {
			auto&& str = classCollection.serialize();
			file.write(str.data(), str.size());
		}
		file.close();
	}
	void File::close()
	{
		save();
		filepath = "";
		classCollection = ClassCollection();
	}
}