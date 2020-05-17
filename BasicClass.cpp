#include "pch.h"
#include "BasicClass.h"
#include "StringFunctions.h"
#include <optional>


namespace jbon {
	BasicClass::BasicClass() {}

	BasicClass::BasicClass(std::string_view schema)
	{
		auto [title, body] = splitTitleBody(schema, '{', '}');
		this->className = title;
		split(keys, std::string_view(body.data() + 1, body.size() - 2), ',');
	}
	std::string BasicClass::serialize() const
	{
		//make class schema
		std::string classSchema,
			body;
		//for the keys, make a giant string of them
		for (int i = 0; i < keys.size(); i++) {
			if (i != keys.size() - 1) {
				body.append(keys[i]);
				body.append(",\n", 2);
			}
			else {
				body.append(keys[i]);
				body.push_back(',');
			}
		}
		//indent all the keys
		indent(body, 4);
		//add the closing brackets and attach to classSchema
		classSchema.reserve(className.size() + body.size() + 5);
		classSchema.append(className);
		classSchema.append(" {\n", 3);
		classSchema.append(body);
		classSchema.append("}\n", 2);
		return classSchema;
	}
	std::string BasicClass::name() const
	{
		return className;
	}
	void parseObject(std::vector<Value>& values, std::string_view str, std::optional<std::string>& name)
	{
		//variables
		auto [title, body] = splitTitleBody(str,'(', ')');

		//see if it has its own name, otherwise nothing changes
		for (int i = 0; i < title.size(); i++) {
			if (title[i] == ':') { //colon is used to specify name
				name = title.substr(i + 1, title.size() - i + 1);
				title = title.substr(0, i);
				//Dog:Scruffles -> title = Dog, objectName = Scruffles
			}
		}
		//makes all the class objects from file by parsing values
		std::vector<std::string_view> fields;
		jbonSplit(fields, body.substr(1, body.size() - 2), ',');
		values.reserve(fields.size());
		for (auto& field : fields) {
			values.push_back(Value::parse(field));
		}
	}
}