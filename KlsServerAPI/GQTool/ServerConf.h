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
	
	void readUserConfInit(std::string argConfFile="");		//�������ļ���׼���ȡ�û���Ϣ
	bool initServer(u_short argPort);						//��ʼ��������
	void acceptManage();									//���ӹ���
	bool checkUserIP( std::string argIP );					//�ض�IP���
	bool checkRechargeIP(std::string argIP);				//��ֵIP���
	int getSock_line(SOCKET argSock, char * argStr, int size);	//��ָ��socket�ж�ȡһ������
	bool elementInspect(ClientInfo * curClient);			//����Ч��
	bool rechargeInspect(ClientInfo * curClient);			//����Ч��
	int requistExecute(ClientInfo * curClient);			//��������ʽѡ������
	void showExcResult(int resultCode);					//��ʾִ�н��
	bool equipCheck(ClientInfo * curClient);			//װ����ѯ����
	bool timeMoniter(ClientInfo * curClient);			//���������Ϣ��ѯ
	bool rechargeOperate(ClientInfo * curClient);		//��ֵ����
	bool flushServerData(ClientInfo * curClient);		//ִ�з���˲���ָ��
	int selectOnGameCharacter(ClientInfo * curClient);	//��ѯ�������Ϸ�е�����
	int selectPastLog(ClientInfo * curClient);			//��ѯ��ʷ�����������
	bool cxecCmd(const char * cmd, char * pBuffer, unsigned int pBufferLen);		//ִ��CMD����
	bool accountInfo(ClientInfo * curClient);			//�û��˺���Ϣ��ѯ
	bool timeInspect(ClientInfo * curClient);			//ʱ���У��
	ClientInfo * saveClientLink(ClientInfo * argClient);		//�洢�ͻ��˵�����
	
public:
	ClientInfo m_Client[MAXCLIENTSIZE];

private:
	SOCKET m_sockServer;
	SQLserverOperate m_SQLServer;

	std::string m_errorStr;			//������ʾ��Ϣ
	std::string m_succesStr;		//��ȷ��ʾ��Ϣ

	std::list< std::string > m_listUserIP;								//׼����û�IP
	std::list< std::string > m_listSign;								//׼���û�sign
	std::list< std::string > m_listAccout;								//׼���װ����ѯ����
	std::list< std::string > m_listEquip;								//׼���û�װ�����Ͳ���
	std::list< std::string > m_listTimer;								//׼���û�ʱ���ѯ���Ͳ���
	std::list< std::string > m_listPort;								//׼���û����������Ķ˿ڡ�ÿ���˿ڶ�Ӧ�ڷ�����·������

	std::list< std::string > m_listRechargeIP;							//׼���ֵIP
	std::list< std::string > m_listRechargeSign;						//׼���ֵsign
	std::list< std::string > m_listRecharge;							//׼���û���ֵ����

	int runPath;														//��ǰ��������   0=����  1=����  2=��ʽ  3=����
	GMAdmin GMInfo;	//�洢 �������Ա��Ϣ

	std::map< std::string, std::string > m_mapSql;						//�洢���ݿ�Ķ���
};