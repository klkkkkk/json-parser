#include <iostream>
#include <fstream>
#include <sstream>
#include "main.h"
#include <utility>
#include <queue>
#include <ctime>
#include "json1/json1.h"
#include "json1/parser.h"
using namespace std;
using json::JsonData;
using json::Parser;
//#include "json/json.h"
//#include "json/json-forwards.h"
//#include "json/jsoncpp.cpp"

int main() {
	ifstream fin("./test.json");
	stringstream ss;
	ss << fin.rdbuf();
	const string& str = ss.str();
	JsonData a(JsonData::json_object);
	clock_t startTime, endTime;
	startTime = clock();//计时开始
	int count = 10000;
	//a["fuck"] = 8;
	try
	{
		for (int i = 0; i < count; i++) {
			a.parse(str);
			if (i < count-1) {
				a.clear();
			}
		}
		cout << a.to_s() <<endl;
		//cout << a.summary();
		//cout << a.to_s();
	}
	catch (const std::exception& e)
	{
		cout << e.what();
	}
	endTime = clock();//计时结束
	cout << fixed;
	cout.precision(6);
	cout << "The run time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	//43.54s/100000t < 41s/100000t
	//Json::Reader jsonReader;
	//Json::Value jsonValue;
	//std::ifstream jsonFile("./test.json");
	//// jsonFile.open("./test.json",std::ios_base::out);
	//clock_t startTime, endTime;
	//startTime = clock();//计时开始
	//for (int i = 0; i < 100000; i++) {
	//	if (!jsonReader.parse(str, jsonValue, true))
	//	{
	//		std::cout << "read error" << std::endl;
	//	}
	//}
	//endTime = clock();//计时结束
	//cout << fixed;
	//cout.precision(6);
	//cout << "The run time is: " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	//Json::Value test = jsonValue["test"];
	//std::cout << test << std::endl;
}