#include <utility>
#include <iostream>
#include "parser.h"
#include <sstream>
#include <Windows.h>
using namespace json;

Parser::Parser(): m_str(""),m_idx(0) {}
void Parser::load(const string& str) {
	m_str = str;
	m_idx = 0;
}
void Parser::skip() {
	while (m_str[m_idx] == ' ' or m_str[m_idx] == '\n' or m_str[m_idx] == '\r' or m_str[m_idx] == '\t') {
		m_idx++;
	}
}
char Parser::get_cur_char() {
	skip();
	return m_str[m_idx];
}
JsonData Parser::parse_null() {
	if (m_str.compare(m_idx, 4, "null") == 0) {
		m_idx += 4;
		return JsonData(JsonData::json_null);
	}
	throw std::logic_error("Bad null json!");
}
JsonData Parser::parse_bool() {
	if (m_str.compare(m_idx, 4, "true") == 0) {
		m_idx += 4;
		return JsonData(true);
	}
	if (m_str.compare(m_idx, 5, "false") == 0) {
		m_idx += 5;
		return JsonData(false);
	}
	throw std::logic_error("Bad bool json!");
}
JsonData Parser::parse_number() {
	int start_pos = m_idx;
	if (m_str[m_idx] == '-') {
		m_idx++;
	}
	if (m_str[m_idx] < '0' or m_str[m_idx] > '9') {
		throw std::logic_error("Bad number json!");
	}
	while (m_str[m_idx] >= '0' and m_str[m_idx] <= '9') {
		m_idx++;
	}
	if (m_str[m_idx] != '.') {
		int i = std::atoi(m_str.c_str() + start_pos);
		return JsonData(i);
	}
	m_idx++;
	if (m_str[m_idx] < '0' or m_str[m_idx] > '9') {
		throw std::logic_error("Bad number json!");
	}
	while (m_str[m_idx] >= '0' and m_str[m_idx] <= '9') {
		m_idx++;
	}
	double i = std::atof(m_str.c_str() + start_pos);
	return JsonData(i);
}
string Parser::parse_string() {//不直接返回JsonData对象，因为对象也要用字符串
	string temp;
	m_idx++;
	while (m_str[m_idx] != '\"' and m_str[m_idx] != '\0'){
		if (m_str[m_idx] == '\\') {
			m_idx++;
			switch (m_str[m_idx]) {
			case '\\': temp += '\\'; break;
			case 'b': temp += '\b'; break;
			case 't': temp += '\t'; break;
			case 'f': temp += '\f'; break;
			case 'n': temp += '\n'; break;
			case 'r': temp += '\r'; break;
			case 'u': {
				std::stringstream ss;
				for (int i = 1; i <= 4; i++) {
					ss << std::hex << m_str[m_idx + i]; //使用字符流将后四位数字以16进制储存
				}
				int code;
				ss >> code;
				if (code < 0x0000 or code >= 0xFFFF) {
					throw std::logic_error("Bad unicode!");
				}
				string result;
				if (code <= 0x7f) {
					result.resize(1);
					result[0] = static_cast<char>(code);
				}
				else if (code <= 0x7FF) {
					result.resize(2);
					result[1] = static_cast<char>(0x80 | (0x3f & code));
					result[0] = static_cast<char>(0xC0 | (0x1f & (code >> 6)));
				}
				else if (code <= 0xFFFF) {
					result.resize(3);
					result[2] = static_cast<char>(0x80 | (0x3f & code));
					result[1] = static_cast<char>(0x80 | (0x3f & (code >> 6)));
					result[0] = static_cast<char>(0xE0 | (0xf & (code >> 12)));
				}
				temp += result;//将编码转化为unicode字符
				m_idx += 4;
				break; }
			default: temp += m_str[m_idx]; break;
			}
		}
		else {
			temp += m_str[m_idx];
		}
		m_idx++;
	}
	skip();
	m_idx++;//字符串后面的逗号
	return temp;
}
JsonData Parser::parse_array() {
	JsonData arr(JsonData::json_array);
	m_idx++;
	if (m_str[m_idx] == ']') {
		return arr;
	}
	else {
		while (true) {
			arr.append(parse());
			if (m_str[m_idx] == ']') {
				m_idx++;
				break;
			}
			if (m_str[m_idx] != ',') {
				throw std::logic_error("Bad array json!");
			}
			skip();
			m_idx++;
		}
	}
	return arr;
}
JsonData Parser::parse_object() {
	JsonData obj(JsonData::json_object);
	m_idx++;//进入括号内容
	skip();
	if (m_str[m_idx] == '}') {
		return obj;
	}
	string key;
	while (true) {
		if (m_str[m_idx] != '\"') {//解析key
			throw std::logic_error("Bad object key!");
		}
		key = parse_string();
		skip();
		if (m_str[m_idx] != ':') {
			throw std::logic_error("Bad object json!");
		}
		m_idx++;
		skip();
		obj[key] = parse();//解析value
		skip();
		if (m_str[m_idx] == '}') {
			m_idx++;
			break;
		}
		if (m_str[m_idx] != ',') {
			throw std::logic_error("Bad object json!");
		}
		//至此解析完对象中的一个元素
		m_idx++;
		skip();
	}
	return obj;
}
JsonData Parser::parse() {
	char ch = get_cur_char();
	if (ch == 'n') {
		return parse_null();
	}
	if (ch == 't' or ch == 'f') {
		return parse_bool();
	}
	if ((ch >= '0' and ch <= '9') or ch == '-') {
		return parse_number();
	}
	if (ch == '\"') {
		return JsonData(parse_string());
	}
	if (ch == '[') {
		return parse_array();
	}
	if (ch == '{') {
		return parse_object();
	}
	throw std::logic_error("Unexpected char!");
}