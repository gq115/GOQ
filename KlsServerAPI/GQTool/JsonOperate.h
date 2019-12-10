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

	//�� Json �ļ��з��� Json ��ʽ�ַ���
	static std::string readJsonFromFile(const std::string argFileName);

	//�� Json ��ʽ�ַ�������д�뵽 Json �ļ���
	static bool writeJsonToFile(const Json::Value & root, const std::string argFileName);

	//�� Json ��ʽ�ַ���ת���� Json::Value ����
	static Json::Value stringParseJson(const std::string & argString);

	//��ָ�� map ��ʽת���� Json ����
	static Json::Value mapParseJson(const std::map<std::string, std::string> argMap);
	//��ָ�� multimap ��ʽת���� Json �ַ���
	static std::string multimapParseJson(const std::multimap<std::string, std::string> argMap);

	//�� Json::Value ����ת��Ϊ Json ��ʽ�ַ���
	static std::string jsonParseString(Json::Value & root);


	//���� json::value ����
	static void jsonPrint(Json::Value & root);
};

