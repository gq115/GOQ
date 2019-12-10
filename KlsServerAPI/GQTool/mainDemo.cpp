#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include "JsonOperate.h"
#include "MD5Operate.h"
#include "SQLserverOperate.h"


using namespace std;

void testJsonOperate();
void testMD5Operate();
void testSQLOperate();

int main1() {

	//testJsonOperate();		//OK
	//testMD5Operate();			//OK

	getchar();
	return 0;
}

void testJsonOperate() {
	const char * tempStr = "{	\"name\": \"json\",			\
								\"array\" : [\"123\",		\
								\"456\",\"789\"] }";
	JsonOperate opt;
	Json::Value root = opt.stringParseJson(tempStr);
	cout << "name:" << root["name"].asString() << endl;
	cout << "array:" << root["array"] << endl;
}

void testMD5Operate() {
	string str("asd3b1S2Bib2suygf1dfgdfgiaehfofnviehiusvbvisauefviuhsiduf");
	char *p = (char *)str.c_str();
	MD5Operate md5;
	md5.GenerateMD5((unsigned char *)p, str.length());
	cout << md5.ToString() << endl;
	cout << md5.md5(str) << endl;
}

void testSQLOperate() {
	//不能链接，无法测试
}