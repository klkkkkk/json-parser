#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>

namespace json {
	using std::string;
	using std::vector;
	using std::map;

	class JsonData {
	public:
		enum DataType { json_null, json_bool, json_int, json_double, json_string, json_array, json_object };
	private:
		DataType m_type;
		union Value {
			bool m_bool;
			int m_int;
			double m_double;
			string* m_string;
			vector<JsonData>* m_array;
			map<string, JsonData>* m_object;
			/*Value() { m_bool = false; }
			~Value() {};*/
		};
		Value m_value;
	public:
		JsonData();
		JsonData(bool v);
		JsonData(int v);
		JsonData(double v);
		JsonData(const char* v);
		JsonData(const string& v);
		JsonData(DataType type);
		JsonData(const JsonData& v);
		JsonData(JsonData&& v);
		void operator=(const JsonData& v);
		void operator=(JsonData&& v);
		JsonData& operator[](int index);
		JsonData& operator[](const char* key);
		JsonData& operator[](const string& key);
		bool operator==(const JsonData& v) const;
		bool operator!=(const JsonData& v) const;
		void append(const JsonData& v);
		operator bool() const;
		operator int() const;
		operator double() const;
		operator string() const;
		string to_s() const;//展示json内容
		string summary(int stack=0) const;//统计json中各种类型数据数量
		bool isnull() const { return m_type == json_null; }
		bool isbool() const { return m_type == json_bool; }
		bool isint() const { return m_type == json_int; }
		bool isdouble() const { return m_type == json_double; }
		bool isstring() const { return m_type == json_string; }
		bool isarray() const { return m_type == json_array; }
		bool isobject() const { return m_type == json_object; }
		size_t size() const;
		bool haskey(const char* key) const;
		bool haskey(const string& key) const;
		void remove(int index);
		void remove(const char* key);
		void remove(const string& key);
		void parse(const string& str);
		void clear();
		~JsonData();

		std::vector<JsonData>::iterator begin() {
			return (m_value.m_array)->begin();
		}
		std::vector<JsonData>::iterator end() {
			return (m_value.m_array)->end();
		}
	};
}