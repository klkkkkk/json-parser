#include <utility>
#include <iostream>
#include "json1.h"
#include "parser.h"
#include <sstream>
using namespace json;

JsonData::JsonData(): m_type(json_null){
	//std::cout << "default" << std::endl;
	m_value.m_bool = false;
}
JsonData::JsonData(bool v): m_type(json_bool) {
	m_value.m_bool = v;
}
JsonData::JsonData(int v) : m_type(json_int) {
	m_value.m_int = v;
}
JsonData::JsonData(double v) : m_type(json_double) {
	m_value.m_double = v;
}
JsonData::JsonData(const char* v) : m_type(json_string) {
	m_value.m_string = new string(v);
}
JsonData::JsonData(const string& v) : m_type(json_string) {
	m_value.m_string = new string(v);
}
JsonData::JsonData(DataType type) : m_type(type) {
	switch (type) {
	case json_null:
		m_value.m_bool = false;
		break;
	case json_bool:
		m_value.m_bool = true;
		break;
	case json_int:
		m_value.m_int = 0;
		break;
	case json_double:
		m_value.m_double = 0.0;
		break;
	case json_string:
		m_value.m_string = new string("");
		break;
	case json_array:
		m_value.m_array = new std::vector<JsonData>();
		break;
	case json_object:
		m_value.m_object = new std::map<string,JsonData>();
		break;
	}
}
JsonData::JsonData(const JsonData& v) : m_type(v.m_type) {
	//std::cout << "copy:" << m_type << std::endl;
	switch (v.m_type) {
	case json_null:
		m_value.m_bool = false;
		break;
	case json_bool:
		m_value.m_bool = v.m_value.m_bool;
		break;
	case json_int:
		m_value.m_int = v.m_value.m_int;
		break;
	case json_double:
		m_value.m_double = v.m_value.m_double;
		break;
	case json_string:
		m_value.m_string = new string(*(v.m_value.m_string));
		break;
	case json_array:
		m_value.m_array = new vector<JsonData>(*(v.m_value.m_array)); //v.m_value.m_array;
		break;
	case json_object:
		m_value.m_object = new map<string, JsonData>(*(v.m_value.m_object));
		break;
	}
}
JsonData::JsonData(JsonData&& v): m_type(v.m_type) {
	//std::cout << "move:" << m_type << std::endl;
	switch (v.m_type) {
	case json_null:
		m_value.m_bool = false;
		break;
	case json_bool:
		m_value.m_bool = v.m_value.m_bool;
		break;
	case json_int:
		m_value.m_int = v.m_value.m_int;
		break;
	case json_double:
		m_value.m_double = v.m_value.m_double;
		break;
	case json_string:
		m_value.m_string = v.m_value.m_string;
		v.m_value.m_string = nullptr;
		break;
	case json_array:
		m_value.m_array = v.m_value.m_array;
		v.m_value.m_array = nullptr;
		break;
	case json_object:
		m_value.m_object = v.m_value.m_object;
		v.m_value.m_object = nullptr;
		break;
	}
}

void JsonData::operator=(const JsonData& v) {
	//std::cout << "lvalue ="<<std::endl;
	if (this == &v) {
		return;
	}
	m_type = v.m_type;
	switch (v.m_type) {
	case json_null:
		m_value.m_bool = false;
		break;
	case json_bool:
		m_value.m_bool = v.m_value.m_bool;
		break;
	case json_int:
		m_value.m_int = v.m_value.m_int;
		break;
	case json_double:
		m_value.m_double = v.m_value.m_double;
		break;
	case json_string:
		m_value.m_string = new string(*(v.m_value.m_string));
		break;
	case json_array:
		m_value.m_array = new vector<JsonData>(*(v.m_value.m_array));
		break;
	case json_object:
		m_value.m_object = new map<string, JsonData>(*(v.m_value.m_object));
		break;
	}
}
void JsonData::operator=(JsonData&& v){
	//std::cout << "rvalue =" << std::endl;
	if (this == &v) {
		return;
	}
	m_type = v.m_type;
	switch (v.m_type) {
	case json_null:
		m_value.m_bool = false;
		break;
	case json_bool:
		m_value.m_bool = v.m_value.m_bool;
		break;
	case json_int:
		m_value.m_int = v.m_value.m_int;
		break;
	case json_double:
		m_value.m_double = v.m_value.m_double;
		break;
	case json_string:
		//delete m_value.m_string;
		m_value.m_string = v.m_value.m_string;
		v.m_value.m_string = nullptr;
		break;
	case json_array:
		//delete m_value.m_array;
		m_value.m_array = v.m_value.m_array;
		v.m_value.m_array = nullptr;
		break;
	case json_object:
		//delete m_value.m_object;
		m_value.m_object = v.m_value.m_object;
		v.m_value.m_object = nullptr;
		break;
	}
}
JsonData& JsonData::operator[](int index) {
	if (m_type != json_array) {
		throw std::logic_error("Not an array!");
	}
	if (index < 0) {
		throw std::logic_error("Index less than 0!");
	}
	size_t size = (m_value.m_array)->size();
	if (index >= size) {
		throw std::logic_error("Index more than size(" + std::to_string(size) + ")!");
	}
	return (m_value.m_array)->at(index);
}
JsonData& JsonData::operator[](const char* key) {
	return (*this)[string(key)];
}
JsonData& JsonData::operator[](const string& key) {
	if (m_type != json_object) {
		throw std::logic_error("Not an object!");
	}
	return (*(m_value.m_object))[key];
}
bool JsonData::operator==(const JsonData& v) const {
	if (m_type != v.m_type) {
		return false;
	}
	switch (m_type) {
	case json_null:
		return true;
	case json_bool:
		return (m_value.m_bool == v.m_value.m_bool);
	case json_int:
		return (m_value.m_int == v.m_value.m_int);
	case json_double:
		return (m_value.m_double == v.m_value.m_double);
	case json_string:
		return !((m_value.m_string)->compare(*(v.m_value.m_string)));
	case json_array:
		return (*(m_value.m_array)==*(v.m_value.m_array));
	case json_object:
		if ((m_value.m_object)->size() != (v.m_value.m_object)->size()) {
			return false;
		}
		for (auto& it1 : *(m_value.m_object)) {
			for (auto& it2 : *(v.m_value.m_object)) {
				if (it1.first.compare(it2.first)) {
					return false;
				}
				if (it1.second != it2.second) {
					return false;
				}
			}
		}
	}
	return true;
}
bool JsonData::operator!=(const JsonData& v) const {
	return !((*this) == v);
}
void JsonData::append(const JsonData& v) {
	if (this == &v) {
		throw std::logic_error("Can't append self!");
	}
	if (m_type != json_array) {//对一个非数组对象拼接时，先将其变为一个元素的数组，再拼接
		if (m_type == json_null) {
			m_value.m_array = new vector<JsonData>();
		}
		else {
			JsonData temp(std::move(*this));
			m_value.m_array = new vector<JsonData>();
			(m_value.m_array)->push_back(temp);
		}
		m_type = json_array;
	}
	(m_value.m_array)->push_back(v);
}
//将一个JsonData转换为其对应格式的数据
JsonData::operator bool() const{
	if (m_type != json_bool) {
		throw std::logic_error("Type error! Not bool type.");
	}
	return m_value.m_bool;
}
JsonData::operator int() const{
	if (m_type != json_int) {
		throw std::logic_error("Type error! Not int type.");
	}
	return m_value.m_int;
}
JsonData::operator double() const{
	if (m_type != json_double) {
		throw std::logic_error("Type error! Not double type.");
	}
	return m_value.m_double;
}
JsonData::operator string() const{
	if (m_type != json_string) {
		throw std::logic_error("Type error! Not string type.");
	}
	return *(m_value.m_string);
}
string JsonData::to_s() const{
	std::stringstream ss;
	switch (m_type) {
	case json_null:
		ss << "null";
		break;
	case json_bool:
		if (m_value.m_bool) {
			ss << "true";
		}
		else {
			ss << "false";
		}
		break;
	case json_int:
		ss << m_value.m_int;
		break;
	case json_double:
		ss << m_value.m_double;
		break;
	case json_string:
		ss << '\"' << * (m_value.m_string) << '\"';
		break;
	case json_array:
		ss << '[';
		for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++) {
			if (it != (m_value.m_array)->begin()) {
				ss << ',';
			}
			ss << it->to_s();
		}
		ss << ']';
		break;
	case json_object:
		ss << '{';
		for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++) {
			if (it != (m_value.m_object)->begin()) {
				ss << ',';
			}
			ss << '\"' << it->first << '\"'<< ':' << it->second.to_s();
		}
		ss << '}';
		break;
	}
	return ss.str();
}
string JsonData::summary(int stack) const {
	int null_val = 0;
	int bool_val = 0;
	int int_val = 0;
	int double_val = 0;
	int string_val = 0;
	int array_val = 0;
	int object_val = 0;
	std::stringstream ss;

	if (m_type == json_array) {
		for (int i = 0; i < stack; i++) {//处理缩进
			ss << '\t';
		}
		ss << "array:[\n";
		for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++) {
			if (it->m_type == json_null) {
				null_val++;
			}
			if (it->m_type == json_bool) {
				bool_val++;
			}
			if (it->m_type == json_int) {
				int_val++;
			}
			if (it->m_type == json_double) {
				double_val++;
			}
			if (it->m_type == json_string) {
				string_val++;
			}
			if (it->m_type == json_array) {
				array_val++;
			}
			if (it->m_type == json_object) {
				object_val++;
			}
		}
		if (null_val > 0) {
			for (int i = 0; i < stack; i++) {
				ss << '\t';
			}
			ss << "null: " << null_val << '\n';
		}
		if (bool_val > 0) {
			for (int i = 0; i < stack; i++) {
				ss << '\t';
			}
			ss << "bool: " << bool_val << '\n';
		}
		if (int_val > 0) {
			for (int i = 0; i < stack; i++) {
				ss << '\t';
			}
			ss << "int: " << int_val << '\n';
		}
		if (double_val > 0) {
			for (int i = 0; i < stack; i++) {
				ss << '\t';
			}
			ss << "double: " << double_val << '\n';
		}
		if (string_val > 0) {
			for (int i = 0; i < stack; i++) {
				ss << '\t';
			}
			ss << "string: " << string_val << '\n';
		}
		if (array_val > 0) {
			for (int i = 0; i < stack; i++) {
				ss << '\t';	
			}
			ss << "array: " << array_val << '\n';
		}
		for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++) {
			if (it->m_type == json_array) {//数组嵌套数组
				ss << it->summary(stack+1);
			}
		}
		if (object_val > 0) {
			for (int i = 0; i < stack; i++) {
				ss << '\t';
			}
			ss << "object: " << object_val << '\n';
		}
		for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++) {
			if (it->m_type == json_object) {//数组嵌套对象
				ss << it->summary(stack + 1);
			}
		}
		for (int i = 0; i < stack; i++) {
			ss << '\t';
		}
		ss << ']' << '\n';
	}
	if (m_type == json_object) {
		for (int i = 0; i < stack; i++) {
			ss << '\t';
		}
		ss << "object:{\n";
		for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++) {
			if (it->second.m_type == json_null) {
				null_val++;
			}
			if (it->second.m_type == json_bool) {
				bool_val++;
			}
			if (it->second.m_type == json_int) {
				int_val++;
			}
			if (it->second.m_type == json_double) {
				double_val++;
			}
			if (it->second.m_type == json_string) {
				string_val++;
			}
			if (it->second.m_type == json_array) {
				array_val++;
			}
			if (it->second.m_type == json_object) {
				object_val++;
			}
		}
		if (null_val > 0) {
			for (int i = 0; i < stack; i++) {
				ss << '\t';
			}
			ss << "null: " << null_val << '\n';
		}
		if (bool_val > 0) {
			for (int i = 0; i < stack; i++) {
				ss << '\t';
			}
			ss << "bool: " << bool_val << '\n';
		}
		if (int_val > 0) {
			for (int i = 0; i < stack; i++) {
				ss << '\t';
			}
			ss << "int: " << int_val << '\n';
		}
		if (double_val > 0) {
			for (int i = 0; i < stack; i++) {
				ss << '\t';
			}
			ss << "double: " << double_val << '\n';
		}
		if (string_val > 0) {
			for (int i = 0; i < stack; i++) {
				ss << '\t';
			}
			ss << "string: " << string_val << '\n';
		}
		if (array_val > 0) {
			for (int i = 0; i < stack; i++) {
				ss << '\t';
			}
			ss << "array: " << array_val << '\n';
		}
		for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++) {
			if (it->second.m_type == json_array) {//对象嵌套数组
				ss << it->second.summary(stack + 1);
			}
		}
		if (object_val > 0) {
			for (int i = 0; i < stack; i++) {
				ss << '\t';
			}
			ss << "object: " << object_val << '\n';
		}
		for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++) {
			if (it->second.m_type == json_object) {//对象嵌套对象
				ss << it->second.summary(stack + 1);
			}
		}
		for (int i = 0; i < stack; i++) {
			ss << '\t';
		}
		ss << '}' << '\n';
	}
	return ss.str();
}
size_t JsonData::size() const {
	if (m_type == json_array)
		return (m_value.m_array)->size();
	else if (m_type == json_object)
		return (m_value.m_object)->size();
	else
		return 1;
}
bool JsonData::haskey(const char* key) const {
	return haskey(string(key));
}
bool JsonData::haskey(const string& key) const {
	if (m_type != json_object) {
		return false;
	}
	return ((m_value.m_object)->find(key) != (m_value.m_object)->end());
}
void JsonData::remove(int index) {
	if (m_type != json_array) {
		throw std::logic_error("Type error! Not array type.");
	}
	if (index < 0 or index >= size()) {
		throw std::logic_error("Index error!");
	}
	//(*(m_value.m_array))[index].clear();
	(m_value.m_array)->erase((m_value.m_array)->begin() + index);
}
void JsonData::remove(const char* key) {
	remove(string(key));
}
void JsonData::remove(const string& key) {
	if (m_type != json_object) {
		throw std::logic_error("Type error! Not object type.");
	}
	if (!(haskey(key))) {
		return;
	}
	//(*(m_value.m_object))[key].clear();
	(m_value.m_object)->erase(key);
}
void JsonData::parse(const string& str) {
	Parser p;
	p.load(str);
	*this = p.parse();
}
void JsonData::clear() {
	if (m_type == json_string) {
		delete m_value.m_string;
	}
	else if (m_type == json_array) {
		/*for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++) {
			it->clear();
		}*/
		delete m_value.m_array;
	}
	else if (m_type == json_object) {
		/*for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++) {
			it->second.clear();
		}*/
		delete m_value.m_object;
	}
}
JsonData::~JsonData() {
	//std::cout << "xigo:"<< m_type << std::endl;
	clear();
}