#include "JsonOperate.h"
using namespace std;


JsonOperate::JsonOperate()
{
}


JsonOperate::~JsonOperate()
{
}

/*
	功能：从指定文件中读取 Json 格式字符串数据
	参数：文件名
*/
string JsonOperate::readJsonFromFile(const std::string argFileName) {
	Json::Value root;
	Json::Reader reader;
	ifstream ifs;
	ifs.open(argFileName);

	if ( !reader.parse(ifs, root) ) {
		return "";
	}
	else{
		return root.toStyledString();
	}
}


/*
	功能：将 Json::Value 对象写入到指定的文件中
	参数：Json::Value对象， 文件名
*/
bool JsonOperate::writeJsonToFile(const Json::Value & root, const std::string argFileName) {
	ofstream ofs;
	ofs.open(argFileName);
	ofs << root.toStyledString();
	ofs.close();
	return true;
}


/*
	功能：将 Json 格式字符串数据转换为 Json::Value 对象
	参数：Json格式字符串
*/
Json::Value JsonOperate::stringParseJson(const std::string & argString) {
	Json::Reader reader;
	Json::Value root;
	reader.parse( argString, root );
	return root;
}


/*
	功能：将 Json::Value 对象转换 Json 格式字符串
	参数：Json::Value对象
*/
string JsonOperate::jsonParseString(Json::Value & root) {
	string tempStr = root.toStyledString();
	return tempStr;
}


/*
	功能：以指定 map 形式转换成 Json 对象
	参数：map数据
*/
Json::Value JsonOperate::mapParseJson(const map<string, string> argMap) {
	Json::Value root;

	map<string, string>::const_iterator iter;
	for (iter = argMap.begin(); iter != argMap.end(); iter++)
	{
		root[iter->first] = iter->second;
	}
	return root;
}


/*
	功能：以指定 multimap 形式转换成 Json 字符串
	参数：multimap数据
*/
string JsonOperate::multimapParseJson(const multimap<string, string> argMultiMap) {

	string tempStr = "";
	int pos;
	Json::Value root;
	typedef multimap<string, string>::const_iterator miter;
	miter tempIter = argMultiMap.begin();
	for ( ; tempIter != argMultiMap.end(); tempIter++) {
		pos = 0;
		if ( tempStr == tempIter->first) {
			continue;
		}
		pair<miter, miter> pmiter = argMultiMap.equal_range(tempIter->first);
		for ( miter it = pmiter.first; it != pmiter.second; it++ ) {
			root[pos].append( it->second );
			pos++;
		}
		tempStr = tempIter->first;
	}
	return root.toStyledString().c_str();
}


/*
	功能：便利并打印 Json::Value 对象
	参数：Json::Value 对象
*/
void JsonOperate::jsonPrint(Json::Value & root) {
	Json::Value::Members mem = root.getMemberNames();
	Json::Value::Members::iterator iter;
	for (iter = mem.begin(); iter != mem.end(); iter++)
	{
		cout << *iter << "\t";
		if (root[*iter].type() == Json::arrayValue)
		{
			cout << endl;
			auto cnt = root[*iter].size();
			for (auto i = 0; i < cnt; i++){
				cout << "\t";
				jsonPrint(root[*iter][i]);
			}
		}
		else if (root[*iter].type() == Json::objectValue) {
			cout << endl;
			auto cnt = root[*iter].size();
			cout << "\t";
			jsonPrint( root[*iter] );
		}
		else if (root[*iter].type() == Json::stringValue)
		{
			cout << "\t";
			cout << root[*iter].asString() << endl;
		}
		else if (root[*iter].type() == Json::realValue)
		{
			cout << "\t";
			cout << root[*iter].asDouble() << endl;
		}
		else if (root[*iter].type() == Json::uintValue)
		{
			cout << "\t";
			cout << root[*iter].asUInt() << endl;
		}
		else
		{
			cout << "\t";
			cout << root[*iter] << endl;
		}
	}
	return;
}



