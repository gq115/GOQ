/*
	功能：执行数据库的相关操作，数据库增加条目，数据库删除条目，数据库查询信息等操作
	例如：账号角色查询，角色属性装备增删改等
*/

#pragma once
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <exception>
#include <algorithm>
#include <string>
#include <map>
#include <list>

#import "C:\Program Files\Common Files\system\ado\msado15.dll" \
	no_namespace rename("EOF", "adoEOF")

class SQLserverOperate
{
public:
	SQLserverOperate();
	~SQLserverOperate();

	//初始化数据库 账号，密码，服务器ip，数据库名
	bool initSQLServer(const char * ServerIp, const char * DbName, const char *User, const char * Password);

	//查询账号数据
	bool selectAcountInfo(const char * colName, const char * table, const char * argWhere);
	bool selectAcountInfo(const std::list<std::string> & argColList, const char * argTable, const char * argWhere);
	bool selectAcountInfo(const char * argSqlStr);
	
	//执行存储过程
	bool executeStore(std::string argStoStr);
	bool executeSql(std::string argStoStr);
	//处理查询之后的数据
	void resolveSelectInfo();

	//添加角色属性数据
	//删除角色属性数据

	//修改角色属性数据
	bool alterRoleInfo(const std::map<std::string, std::string> & argAlterList, const char * argTable, const char * argWhere);

	//获取最近一次错误信息
	inline const std::string & getErrorStr();

	//获取查询结果信息
	inline const std::multimap<std::string, std::string> & getcurSelectInfo();

public:
	std::string m_ErrorStr;					//最后一次执行命令错误字符
private:
	_ConnectionPtr m_qConnect;			//连接对象
	_RecordsetPtr m_qRecordset;			//收集对象
	_CommandPtr m_qCommand;				//执行对象
	std::multimap<std::string, std::string> m_accountInfo;			//存储账户信息
	std::multimap<std::string, std::string> m_selectResult;				//存储查询信息
	
};

/*
	功能：获取查询结果信息
	参数：无
*/
const std::multimap<std::string, std::string> & SQLserverOperate::getcurSelectInfo() {
	return m_selectResult;
}

/*
	功能：获取最近一次错误信息
	参数：无
*/
inline const std::string & SQLserverOperate::getErrorStr() {
	return m_ErrorStr;
}