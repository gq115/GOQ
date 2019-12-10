#pragma once

#include <iostream>
#include <string.h>
#include <string>
#include <fstream>
#include <memory>
#include <list>
#include <map>

#include "json/json.h"

#pragma comment(lib,"json_vc71_libmtd.lib")

#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(disable : 4996)
#endif

class JsonOperate
{
public:
	JsonOperate();
	~JsonOperate();

	//从 Json 文件中返回 Json 格式字符串
	static std::string readJsonFromFile(const std::string argFileName);

	//将 Json 格式字符串内容写入到 Json 文件中
	static bool writeJsonToFile(const Json::Value & root, const std::string argFileName);

	//将 Json 格式字符串转换成 Json::Value 数据
	static Json::Value stringParseJson(const std::string & argString);

	//以指定 map 形式转换成 Json 对象
	static Json::Value mapParseJson(const std::map<std::string, std::string> argMap);
	//以指定 multimap 形式转换成 Json 字符串
	static std::string multimapParseJson(const std::multimap<std::string, std::string> argMap);

	//将 Json::Value 数据转换为 Json 格式字符串
	static std::string jsonParseString(Json::Value & root);


	//遍历 json::value 数据
	static void jsonPrint(Json::Value & root);
};

