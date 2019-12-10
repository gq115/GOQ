#pragma once
#include <iostream>
#include <Windows.h>
#include <process.h>
#include <algorithm>
#include <time.h>
#include <string>
#include <string.h>
#include <cstring>
#include <list>
#include <map>
#include <codecvt>
#include "JsonOperate.h"
#include "SQLserverOperate.h"
#include "MD5Operate.h"
#include "Coding.h"


#define MAXBUFFERSIZE 1024

class ClientInfo {
public:
	ClientInfo();
	~ClientInfo();
	void ClientInfoInit();

	int getSock_line(SOCKET argSock, char * argStr, int size);	//从指定socket中读取一行数据
	void discardheaders();										//清除当前socket中的信息
	bool elementResolve();										//参数分解
	std::string getClientArg(std::string arg);					//获取客户端的某个参数
	
	void unimplemented();		//返回给客户端相关信息
	void requistSuccess();
	void requistFalse();

	//添加复制运算符
	ClientInfo & operator=(const ClientInfo & arg);

public:
	unsigned int m_Timer;			//生存时间
	unsigned int m_Socket;
	unsigned int m_SelectTime;		//主要是为了记录装备插入时和查询时的时间差
	struct sockaddr_in m_sockaddr;
	char m_Buffer[MAXBUFFERSIZE];

	int m_Type;					//存储用户的此次操作的操作类型    1==获取账号信息， 2==发送装备，  3==查询用户在线
	std::string m_strMethod;	//存储用户的请求方式
	std::string m_strPath;		//存储用户的请求路径
	std::map< std::string, std::string > m_mapElement;		//请求参数表
	std::string m_errorStr;
	std::string m_succesStr;
};
