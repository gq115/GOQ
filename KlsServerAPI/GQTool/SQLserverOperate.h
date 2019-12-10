/*
	���ܣ�ִ�����ݿ����ز��������ݿ�������Ŀ�����ݿ�ɾ����Ŀ�����ݿ��ѯ��Ϣ�Ȳ���
	���磺�˺Ž�ɫ��ѯ����ɫ����װ����ɾ�ĵ�
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

	//��ʼ�����ݿ� �˺ţ����룬������ip�����ݿ���
	bool initSQLServer(const char * ServerIp, const char * DbName, const char *User, const char * Password);

	//��ѯ�˺�����
	bool selectAcountInfo(const char * colName, const char * table, const char * argWhere);
	bool selectAcountInfo(const std::list<std::string> & argColList, const char * argTable, const char * argWhere);
	bool selectAcountInfo(const char * argSqlStr);
	
	//ִ�д洢����
	bool executeStore(std::string argStoStr);
	bool executeSql(std::string argStoStr);
	//�����ѯ֮�������
	void resolveSelectInfo();

	//��ӽ�ɫ��������
	//ɾ����ɫ��������

	//�޸Ľ�ɫ��������
	bool alterRoleInfo(const std::map<std::string, std::string> & argAlterList, const char * argTable, const char * argWhere);

	//��ȡ���һ�δ�����Ϣ
	inline const std::string & getErrorStr();

	//��ȡ��ѯ�����Ϣ
	inline const std::multimap<std::string, std::string> & getcurSelectInfo();

public:
	std::string m_ErrorStr;					//���һ��ִ����������ַ�
private:
	_ConnectionPtr m_qConnect;			//���Ӷ���
	_RecordsetPtr m_qRecordset;			//�ռ�����
	_CommandPtr m_qCommand;				//ִ�ж���
	std::multimap<std::string, std::string> m_accountInfo;			//�洢�˻���Ϣ
	std::multimap<std::string, std::string> m_selectResult;				//�洢��ѯ��Ϣ
	
};

/*
	���ܣ���ȡ��ѯ�����Ϣ
	��������
*/
const std::multimap<std::string, std::string> & SQLserverOperate::getcurSelectInfo() {
	return m_selectResult;
}

/*
	���ܣ���ȡ���һ�δ�����Ϣ
	��������
*/
inline const std::string & SQLserverOperate::getErrorStr() {
	return m_ErrorStr;
}