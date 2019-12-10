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
#include "ClientInfo.h"
#include "MAHeader.h"

#define MAXCLIENTSIZE 128

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Psapi.lib")

UINT _stdcall threadAccept_request(LPVOID argLP);

class ServerConf
{
public:
	enum OPERATIONTYPE {
		TYPE_GET = 0,
		TYPE_POST = 100,
		TYPE_PUT = 200,
		TYPE_OTHER = 300
	};
	enum OPERATIONNUM {
		GET_OTHER = 0,
		GET_ACCOUNT_SUCCESS,
		GET_ACCOUNT_FALSE,
		GET_EQUIP_SUCCESS,
		GET_EQUIP_FALSE,
		GET_TIMER_SUCCESS,
		GET_TIMER_FALSE,
		GET_RECHARGE_SUCCESS,
		GET_RECHARGE_FALSE,
		GET_FLUSHDATA_SUCCESS,
		GET_FLUSHDATA_FALSE,
		

		POST_OTHER = 0,

		PUT_OTHER = 0
	};

public:
	ServerConf();
	~ServerConf();
	
	void readUserConfInit(std::string argConfFile="");		//从配置文件中准许读取用户信息
	bool initServer(u_short argPort);						//初始化服务器
	void acceptManage();									//连接管理
	bool checkUserIP( std::string argIP );					//特定IP检测
	bool checkRechargeIP(std::string argIP);				//充值IP检测
	int getSock_line(SOCKET argSock, char * argStr, int size);	//从指定socket中读取一行数据
	bool elementInspect(ClientInfo * curClient);			//参数效验
	bool rechargeInspect(ClientInfo * curClient);			//参数效验
	int requistExecute(ClientInfo * curClient);			//根据请求方式选择请求
	void showExcResult(int resultCode);					//显示执行结果
	bool equipCheck(ClientInfo * curClient);			//装备查询操作
	bool timeMoniter(ClientInfo * curClient);			//玩家在线信息查询
	bool rechargeOperate(ClientInfo * curClient);		//充值操作
	bool flushServerData(ClientInfo * curClient);		//执行服务端操作指令
	int selectOnGameCharacter(ClientInfo * curClient);	//查询玩家在游戏中的数量
	int selectPastLog(ClientInfo * curClient);			//查询历史玩家在线数量
	bool cxecCmd(const char * cmd, char * pBuffer, unsigned int pBufferLen);		//执行CMD命令
	bool accountInfo(ClientInfo * curClient);			//用户账号信息查询
	bool timeInspect(ClientInfo * curClient);			//时间戳校验
	ClientInfo * saveClientLink(ClientInfo * argClient);		//存储客户端的连接
	
public:
	ClientInfo m_Client[MAXCLIENTSIZE];

private:
	SOCKET m_sockServer;
	SQLserverOperate m_SQLServer;

	std::string m_errorStr;			//错误提示信息
	std::string m_succesStr;		//正确提示信息

	std::list< std::string > m_listUserIP;								//准许的用户IP
	std::list< std::string > m_listSign;								//准许用户sign
	std::list< std::string > m_listAccout;								//准许的装备查询参数
	std::list< std::string > m_listEquip;								//准许用户装备发送参数
	std::list< std::string > m_listTimer;								//准许用户时间查询发送参数
	std::list< std::string > m_listPort;								//准许用户检测服务器的端口。每个端口对应于服务线路的链接

	std::list< std::string > m_listRechargeIP;							//准许充值IP
	std::list< std::string > m_listRechargeSign;						//准许充值sign
	std::list< std::string > m_listRecharge;							//准许用户充值参数

	int runPath;														//当前所处环境   0=本地  1=测试  2=正式  3=新区
	GMAdmin GMInfo;	//存储 服务管理员信息

	std::map< std::string, std::string > m_mapSql;						//存储数据库的东西
};