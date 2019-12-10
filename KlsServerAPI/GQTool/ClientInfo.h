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

	int getSock_line(SOCKET argSock, char * argStr, int size);	//��ָ��socket�ж�ȡһ������
	void discardheaders();										//�����ǰsocket�е���Ϣ
	bool elementResolve();										//�����ֽ�
	std::string getClientArg(std::string arg);					//��ȡ�ͻ��˵�ĳ������
	
	void unimplemented();		//���ظ��ͻ��������Ϣ
	void requistSuccess();
	void requistFalse();

	//��Ӹ��������
	ClientInfo & operator=(const ClientInfo & arg);

public:
	unsigned int m_Timer;			//����ʱ��
	unsigned int m_Socket;
	unsigned int m_SelectTime;		//��Ҫ��Ϊ�˼�¼װ������ʱ�Ͳ�ѯʱ��ʱ���
	struct sockaddr_in m_sockaddr;
	char m_Buffer[MAXBUFFERSIZE];

	int m_Type;					//�洢�û��Ĵ˴β����Ĳ�������    1==��ȡ�˺���Ϣ�� 2==����װ����  3==��ѯ�û�����
	std::string m_strMethod;	//�洢�û�������ʽ
	std::string m_strPath;		//�洢�û�������·��
	std::map< std::string, std::string > m_mapElement;		//���������
	std::string m_errorStr;
	std::string m_succesStr;
};
