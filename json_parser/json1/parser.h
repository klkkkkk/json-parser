#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
#include "json1.h"

namespace json {
	using std::string;
	class Parser {
	public:
		Parser();
		void load(const string& str);
		void reset() { m_idx = 0; }
		JsonData parse();
	private:
		string m_str;
		int m_idx;
		JsonData parse_null();
		JsonData parse_bool();
		JsonData parse_number();
		string parse_string();
		JsonData parse_array();
		JsonData parse_object();
		void skip();
		char get_cur_char();
	};
}