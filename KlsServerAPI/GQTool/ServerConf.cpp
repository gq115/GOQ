#include "ServerConf.h"

extern ServerConf *MainServer;

using namespace std;

ServerConf::ServerConf(){
	m_sockServer = INVALID_SOCKET;
}

ServerConf::~ServerConf(){}


/*
	功能：读取用户配置文件，取出特权用户的相关信息。如 IP组，检测组，MD5码组等
	参数：配置文件
*/
void ServerConf::readUserConfInit(string argConfFile) {
	string tempPath = "";
	if ( (argConfFile.length() == 0) || (argConfFile == "") ) {
		tempPath = ".\\userinfo.ini";
	}
	else {
		tempPath = argConfFile;
		int i=0;
		while ( (i = tempPath.find("\\",i)) != -1 ) {
			tempPath.insert( i,"\\" );
			i = i + 2;
		}
	}

	runPath = GetPrivateProfileInt("CurPath", "runPath", 0, tempPath.c_str());

	int tempCount = 1;	
	int tempi = 1;
	string tempLeft;
	char tempRight[128] = "";

	while ( tempCount ) {			//准许用户IP配置
		tempLeft = "IP";
		tempLeft += to_string( tempi );
		tempi++;
		strcpy_s(tempRight, "");
		tempCount = GetPrivateProfileStringA("userIP", tempLeft.c_str(), "", tempRight, 128, tempPath.c_str());
		if ( tempCount ) 
			m_listUserIP.push_back( string(tempRight) );
	}

	tempCount = 1;
	tempi = 1;
	while (tempCount) {		//装备插入参数配置
		tempLeft = "inspect";
		tempLeft += to_string(tempi);
		tempi++;
		strcpy_s(tempRight, "");
		tempCount = GetPrivateProfileStringA("inspectEquip", tempLeft.c_str(), "", tempRight, 128, tempPath.c_str());
		if (tempCount) {
			m_listEquip.push_back(string(tempRight));
		}
	}

	tempCount = 1;
	tempi = 1;
	while (tempCount) {		//账号查询参数
		tempLeft = "inspect";
		tempLeft += to_string(tempi);
		tempi++;
		strcpy_s(tempRight, "");
		tempCount = GetPrivateProfileStringA("inspectRole", tempLeft.c_str(), "", tempRight, 128, tempPath.c_str());
		if (tempCount)
			m_listAccout.push_back( string(tempRight) );
	}

	tempCount = 1;
	tempi = 1;
	while ( tempCount ) {	//准许用户的特有sign
		tempLeft = "sign";
		tempLeft += to_string(tempi);
		tempi++;
		strcpy_s(tempRight, "");
		tempCount = GetPrivateProfileStringA("userConf", tempLeft.c_str(), "", tempRight, 128, tempPath.c_str());
		if (tempCount)
			m_listSign.push_back( string(tempRight) );
	}

	tempCount = 1;
	tempi = 1;
	while ( tempCount ) {	//准许用户的时间监视参数
		tempLeft = "inspect";
		tempLeft += to_string(tempi);
		tempi++;
		strcpy_s(tempRight, "");
		tempCount = GetPrivateProfileStringA("inspectTimer", tempLeft.c_str(), "", tempRight, 128, tempPath.c_str());
		if (tempCount)
			m_listTimer.push_back(string(tempRight));
	}

	tempCount = 1;
	tempi = 1;
	while (tempCount) {	//准许用户的时间监视参数
		tempLeft = "Port";
		tempLeft += to_string(tempi);
		tempi++;
		strcpy_s(tempRight, "");
		tempCount = GetPrivateProfileStringA("ServerInfo", tempLeft.c_str(), "", tempRight, 128, tempPath.c_str());
		if (tempCount)
			m_listPort.push_back(string(tempRight));
	}

	tempCount = 1;
	tempi = 1;
	while (tempCount) {	//准许充值IP参数
		tempLeft = "IP";
		tempLeft += to_string(tempi);
		tempi++;
		strcpy_s(tempRight, "");
		tempCount = GetPrivateProfileStringA("rechargeIP", tempLeft.c_str(), "", tempRight, 128, tempPath.c_str());
		if (tempCount)
			m_listRechargeIP.push_back(string(tempRight));
	}

	tempCount = 1;
	tempi = 1;
	while (tempCount) {	//准许充值sign参数
		tempLeft = "sign";
		tempLeft += to_string(tempi);
		tempi++;
		strcpy_s(tempRight, "");
		tempCount = GetPrivateProfileStringA("rechargeSign", tempLeft.c_str(), "", tempRight, 128, tempPath.c_str());
		if (tempCount)
			m_listRechargeSign.push_back(string(tempRight));
	}

	tempCount = 1;
	tempi = 1;
	while (tempCount) {	//准许充值参数
		tempLeft = "inspect";
		tempLeft += to_string(tempi);
		tempi++;
		strcpy_s(tempRight, "");
		tempCount = GetPrivateProfileStringA("inspectRechage", tempLeft.c_str(), "", tempRight, 128, tempPath.c_str());
		if (tempCount)
			m_listRecharge.push_back(string(tempRight));
	}


	tempLeft = "IP";
	strcpy_s(tempRight, "");
	tempCount = GetPrivateProfileStringA("inspectSql", tempLeft.c_str(), "", tempRight, 128, tempPath.c_str());
	if (tempCount)
		m_mapSql.insert( pair<string,string>(tempLeft, tempRight) );
	tempLeft = "DB";
	strcpy_s(tempRight, "");
	tempCount = GetPrivateProfileStringA("inspectSql", tempLeft.c_str(), "", tempRight, 128, tempPath.c_str());
	if (tempCount)
		m_mapSql.insert(pair<string, string>(tempLeft, tempRight));
	tempLeft = "UN";
	strcpy_s(tempRight, "");
	tempCount = GetPrivateProfileStringA("inspectSql", tempLeft.c_str(), "", tempRight, 128, tempPath.c_str());
	if (tempCount)
		m_mapSql.insert(pair<string, string>(tempLeft, tempRight));
	tempLeft = "UP";
	strcpy_s(tempRight, "");
	tempCount = GetPrivateProfileStringA("inspectSql", tempLeft.c_str(), "", tempRight, 128, tempPath.c_str());
	if (tempCount)
		m_mapSql.insert(pair<string, string>(tempLeft, tempRight));
}


/*
	功能：初始化服务的相关信息。如 IP，端口号等
	参数：端口号
*/
bool ServerConf::initServer( u_short argPort ) {
	u_short tempPort = argPort==0 ? 80 : argPort;
	struct sockaddr_in tempServerAddr;
	WSADATA tempWsadate;
	WSAStartup(MAKEWORD(2, 2), &tempWsadate);
	readUserConfInit( ".\\userinfo.ini" );
	string sqlIP = "127.0.0.1";
	string sqlDB = "CronousData";
	string sqlUN = "sa";
	string sqlUP = "123";
	if (m_mapSql.find("IP") != m_mapSql.end())
		sqlIP = m_mapSql.find("IP")->second;
	if (m_mapSql.find("DB") != m_mapSql.end())
		sqlDB = m_mapSql.find("DB")->second;
	if (m_mapSql.find("UN") != m_mapSql.end())
		sqlUN = m_mapSql.find("UN")->second;
	if (m_mapSql.find("UP") != m_mapSql.end())
		sqlUP = m_mapSql.find("UP")->second;
	if (!m_SQLServer.initSQLServer(sqlIP.c_str(), sqlDB.c_str(), sqlUN.c_str(), sqlUP.c_str())) {
		OutputDebugString("数据库连接异常");
		OutputDebugString(m_SQLServer.m_ErrorStr.c_str());
		printf("数据库连接异常\n");
		printf(m_SQLServer.m_ErrorStr.c_str());
		return false;
	}
	OutputDebugString("sql connect success\n");
	OutputDebugString(sqlIP.c_str());
	OutputDebugString(sqlDB.c_str());
	OutputDebugString(sqlUN.c_str());
	OutputDebugString(sqlUP.c_str());
	printf("sql connect success\n");
	printf(sqlIP.c_str());
	printf("\n");
	printf(sqlDB.c_str());
	printf("\n");
	printf(sqlUN.c_str());
	printf("\n");
	printf(sqlUP.c_str());
	printf("\n");
	m_sockServer = socket( AF_INET, SOCK_STREAM, 0 );
	if (m_sockServer == INVALID_SOCKET ) {
		return false;	
	}
	memset( &tempServerAddr, 0, sizeof(tempServerAddr) );
	tempServerAddr.sin_family = AF_INET;
	tempServerAddr.sin_port = htons(tempPort);
	tempServerAddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	if ( bind(m_sockServer, (struct sockaddr *)&tempServerAddr, sizeof(tempServerAddr) ) < 0 ) {
		return false;
	}

	if ( listen(m_sockServer, 5) < 0 ) {
		return false;
	}
	OutputDebugString("listen success\n");
	printf("listen success\n");
	return true;
}


/*
	功能：服务接受客户端的连接，先检测是否为特定IP，再开启线程
	参数：无
*/
void ServerConf::acceptManage() {
	SOCKET tempClientSock = INVALID_SOCKET;
	struct sockaddr_in tempClientAddr;
	int tempClientAddrLen = sizeof( tempClientAddr );

	while (1)   {
		tempClientSock = accept( m_sockServer, 
				(struct sockaddr *)&tempClientAddr, &tempClientAddrLen);
		if (tempClientSock == INVALID_SOCKET) {
			string str = "accept error: error Code:";
			str += GetLastError();
			OutputDebugString(str.c_str());
			printf(str.c_str());
			return;
		}
		string str = "接收客户端IP: ";
		str += inet_ntoa(tempClientAddr.sin_addr);
		OutputDebugString(str.c_str());
		printf(str.c_str());
		string tempIP = inet_ntoa(tempClientAddr.sin_addr);
		if ( !checkUserIP(tempIP) && !checkRechargeIP(tempIP)) {
			str = "非允许IP: ";
			str += tempIP;
			OutputDebugString(str.c_str());
			printf(str.c_str());
			closesocket(tempClientSock);
			continue;
		}

		HANDLE tempThread = (HANDLE)_beginthreadex(
				NULL, 0, threadAccept_request, &tempClientSock, 0, NULL);
		if (tempThread != NULL) {
			CloseHandle(tempThread);
		}
		else {
			closesocket(tempClientSock);
		}
		Sleep(50);
	}
}


/*
	功能：检测当前 用户信息是否被允许
	参数：待检测IP地址
*/
bool ServerConf::checkUserIP( string argIP ) {
	if ( m_listUserIP.empty() ) {
		return false;
	}

	list<string>::iterator tempListIter;
	for ( tempListIter = m_listUserIP.begin(); 
				tempListIter != m_listUserIP.end(); tempListIter++ ) {
		if (*tempListIter == argIP) {
			return true;
		}
	}

	return false;
}


/*
	功能：检测当前 充值信息是否被允许
	参数：待检测IP地址
*/
bool ServerConf::checkRechargeIP(string argIP) {
	if (m_listRechargeIP.empty()) {
		return false;
	}

	list<string>::iterator tempListIter;
	for (tempListIter = m_listRechargeIP.begin();
		tempListIter != m_listRechargeIP.end(); tempListIter++) {
		if (*tempListIter == argIP) {
			return true;
		}
	}

	return false;
}

/*
	功能：根据不同的请求方式执行不同的操作
	参数：无
	返回值：-1=请求失败，0=非有效请求，1=Get请求成功, 2=Post请求成功
			3=Put执行成功
*/
int ServerConf::requistExecute(ClientInfo * curClient){
	OutputDebugString("requistExecute begin");
	if (curClient->m_strMethod.length() == 0 || curClient->m_strMethod == "") {
		return OPERATIONTYPE::TYPE_OTHER;
	}

	if (!_stricmp(curClient->m_strMethod.c_str(), "GET")) {
		if (curClient->m_Type == 1) {				//角色信息查询
			//检测参数
			OutputDebugString("postpoint3\n");
			ClientInfo * mainClinet = saveClientLink(curClient);
			if (accountInfo(mainClinet)) {
				mainClinet->requistSuccess();
				mainClinet->m_SelectTime = (unsigned int)time(NULL);
				return OPERATIONTYPE::TYPE_GET + OPERATIONNUM::GET_ACCOUNT_SUCCESS;
			}
			else {
				multimap<string, string> tempMap;
				tempMap.insert(pair<string, string>("Check", "False"));
				mainClinet->m_errorStr = JsonOperate::multimapParseJson(tempMap);
				mainClinet->requistFalse();
				return OPERATIONTYPE::TYPE_GET + OPERATIONNUM::GET_ACCOUNT_FALSE;
			}
		}
		else if (curClient->m_Type == 2) {			//装备插入
			ClientInfo * mainClinet = saveClientLink(curClient);
			if (equipCheck(mainClinet)) {
				multimap<string, string> tempMap;
				tempMap.insert(pair<string, string>("Insert", "True"));
				mainClinet->m_succesStr = JsonOperate::multimapParseJson(tempMap);
				mainClinet->requistSuccess();
				mainClinet->m_SelectTime = (unsigned int)time(NULL);
				return OPERATIONTYPE::TYPE_GET + OPERATIONNUM::GET_EQUIP_SUCCESS;
			}
			else {
				multimap<string, string> tempMap;
				tempMap.insert(pair<string, string>("Insert", "False"));
				mainClinet->m_errorStr = JsonOperate::multimapParseJson(tempMap);
				mainClinet->requistFalse();
				return OPERATIONTYPE::TYPE_GET + OPERATIONNUM::GET_EQUIP_FALSE;
			}
		}
		else if (curClient->m_Type == 3) {			//玩家在线查询
			if ( timeMoniter(curClient) ) {
				multimap<string, string> tempMap;
				tempMap.insert(pair<string, string>("Count", curClient->m_succesStr));
				curClient->m_succesStr = JsonOperate::multimapParseJson(tempMap);
				curClient->requistSuccess();
				return OPERATIONTYPE::TYPE_GET + OPERATIONNUM::GET_TIMER_SUCCESS;
			}
			else {
				multimap<string, string> tempMap;
				tempMap.insert(pair<string, string>("Count", "False"));
				curClient->m_errorStr = JsonOperate::multimapParseJson(tempMap);
				curClient->requistFalse();
				return  OPERATIONTYPE::TYPE_GET + OPERATIONNUM::GET_TIMER_FALSE;
			}
		}
		else if (curClient->m_Type == 4) {			//充值操作
			if ( rechargeOperate(curClient) ) {
				string tempResult = JsonOperate::multimapParseJson(m_SQLServer.getcurSelectInfo());
				curClient->m_succesStr = tempResult;
				curClient->requistSuccess();
				string str = "rechargeOperate success!!!    resultCode: ";
				str += tempResult;
				OutputDebugString(str.c_str());
				printf(str.c_str());
				return OPERATIONTYPE::TYPE_GET + OPERATIONNUM::GET_RECHARGE_SUCCESS;
			}
			else {
				multimap<string, string> tempMap;
				tempMap.insert(pair<string, string>("returndata", "-98"));
				curClient->m_errorStr = JsonOperate::multimapParseJson(tempMap);
				curClient->requistFalse();
				return  OPERATIONTYPE::TYPE_GET + OPERATIONNUM::GET_RECHARGE_FALSE;
			}
		}
		else if (curClient->m_Type == 5) {			//刷新服务器操作
			if (flushServerData(curClient)) {
				multimap<string, string> tempMap;
				tempMap.insert(pair<string, string>("returndata", "0"));
				curClient->m_succesStr = JsonOperate::multimapParseJson(tempMap);
				curClient->requistSuccess();
				OutputDebugString("刷新客户端文件成功\n");
				printf("刷新客户端文件成功\n");
				return OPERATIONTYPE::TYPE_GET + OPERATIONNUM::GET_FLUSHDATA_SUCCESS;
			}
			else {
				multimap<string, string> tempMap;
				tempMap.insert(pair<string, string>("returndata", "-1"));
				curClient->m_errorStr = JsonOperate::multimapParseJson(tempMap);
				curClient->requistFalse();
				OutputDebugString("刷新客户端文件失败\n");
				printf("刷新客户端文件失败\n");
				return  OPERATIONTYPE::TYPE_GET + OPERATIONNUM::GET_FLUSHDATA_FALSE;
			}
		}
		else {
			return OPERATIONTYPE::TYPE_GET + OPERATIONNUM::GET_OTHER;
		}
	}
	else if (!_stricmp(curClient->m_strMethod.c_str(), "POST")) {
		;//处理POST请求处理
		return OPERATIONTYPE::TYPE_POST + OPERATIONNUM::POST_OTHER;
	}
	else if (!_stricmp(curClient->m_strMethod.c_str(), "PUT")) {
		;//处理put请求处理
		return OPERATIONTYPE::TYPE_PUT + OPERATIONNUM::PUT_OTHER;
	}
	else {
		curClient->unimplemented();
		return OPERATIONTYPE::TYPE_OTHER;
	}
}


/*
	功能：获取指定 socket 中的一行信息
	参数：指定socket，存储字符串，最大读取字节
	返回值：成功读取字节
*/
int ServerConf::getSock_line(SOCKET argSock, char * argBuf, int argSize) {
	OutputDebugString("getSock_line begin!!!\n");
	int i = 0;
	char c = '\0';
	int n;

	while ((i < argSize - 1) && (c != '\n')){
		n = recv(argSock, &c, 1, 0);
		if (n > 0)
		{
			if (c == '\r')
			{
				n = recv(argSock, &c, 1, MSG_PEEK);
				if ((n > 0) && (c == '\n'))
					recv(argSock, &c, 1, 0);
				else
					c = '\n';
			}
			argBuf[i] = c;
			i++;
		}
		else
			c = '\n';
	}
	argBuf[i] = '\0';
	OutputDebugString("getSock_line end!!!\n");
	return(i);
}


/*
	功能：效验当前的用户参数和规定的校验码参数的MD5码是否匹配
	参数：无
*/
bool ServerConf::elementInspect(ClientInfo * curClient) {
	OutputDebugString("elementInspect begin");
	//用户账号+时间+sign == MD5
	string tempMD5Code = "";
	list<string>::const_iterator iter;
	for (iter = m_listAccout.begin(); iter != m_listAccout.end(); iter++ ) {
		if ( (stricmp(iter->c_str(), "chName") == 0) || (stricmp(iter->c_str(), "AccountName") == 0)) {
			continue;
		}
		if (curClient->m_mapElement.find((*iter).c_str()) == curClient->m_mapElement.end()) {
			OutputDebugString("userinfo 角色查询 标准参数异常\n");
			printf("userinfo 角色查询 标准参数异常\n");
			return false;
		}
	}
	if (curClient->m_mapElement.find("sign") == curClient->m_mapElement.end()) {
		OutputDebugString("userinfo 角色查询 sign异常\n");
		printf("userinfo 角色查询 sign异常\n");
		return false;
	}

	//获取本地sign
	//组件MD5查询串
	for (iter = m_listAccout.begin(); iter != m_listAccout.end(); iter++) {
		//cout << "m_listAccount :" << iter->c_str() << endl;
		string str = "m_listAccount :";
		str += iter->c_str();
		OutputDebugString(str.c_str());
		if ( stricmp(iter->c_str(), "sign") == 0) {
			list<string>::iterator niter = m_listUserIP.begin();
			list<string>::iterator niter1 = m_listSign.begin();

			if ( m_listSign.empty() ) {
				OutputDebugString("postpoint0.1\n");
				printf("m_listSign is empty\n");
				return false;
			}
			//string tempIP = inet_ntoa(tempClientAddr.sin_addr);
			string curClientIP = inet_ntoa( curClient->m_sockaddr.sin_addr );
			OutputDebugString("postpoint1\n");
			while ( niter != m_listUserIP.end()) {
				if (*niter == curClientIP)
					break;
				niter++;
				niter1++;
				if (niter1 == m_listSign.end()) {
					niter1 = m_listSign.begin();
				}
			}
			
			OutputDebugString("postpoint2\n");
			string str = "userinfo sign :";
			str += niter1->c_str();
			OutputDebugString( str.c_str() );
			cout << "userinfo sign:" << niter1->c_str() << endl;
			tempMD5Code += *niter1;
			break;
		}
		tempMD5Code += curClient->getClientArg(*iter);
	}
	string str = "MD子串：";
	str += tempMD5Code;
	cout << "MD子串：" << tempMD5Code << endl;
	OutputDebugString(str.c_str());

	MD5Operate tempMD5Obj;
	tempMD5Obj.GenerateMD5( (unsigned char *)tempMD5Code.c_str(), tempMD5Code.length() );
	//cout << "sign:" << curClient->m_mapElement["sign"] << endl;		//post 1
	str = "sign:";
	str += curClient->m_mapElement["sign"];
	OutputDebugString(str.c_str());

	cout << "MD5:" << tempMD5Obj.ToString() << endl;
	str = "MD5:";
	str += tempMD5Obj.ToString();
	OutputDebugString(str.c_str());

	if ( _stricmp(curClient->m_mapElement["sign"].c_str(), tempMD5Obj.ToString().c_str() ) ) {
		OutputDebugString("角色查询 效验码不匹配\n");
		printf("角色查询 效验码不匹配\n");
		return false;
	}
	
	OutputDebugString("elementInspect end");
	return true;
}


/*
	功能：效验当前的充值参数和规定的校验码参数的MD5码是否匹配
	参数：无
*/
bool ServerConf::rechargeInspect(ClientInfo * curClient) {
	//按顺序的所有参数+Sign = MD5
	string tempMD5Code = "";
	list<string>::const_iterator iter;
	for (iter = m_listRecharge.begin(); iter != m_listRecharge.end(); iter++) {
		if (curClient->m_mapElement.find((*iter).c_str()) == curClient->m_mapElement.end()) {
			OutputDebugString("userinfo 标准参数缺少\n");
			printf("userinfo 标准参数缺少\n");
			return false;
		}
	}

	//获取本地sign
	//组件MD5查询串
	for (iter = m_listRecharge.begin(); iter != m_listRecharge.end(); iter++) {
		if (stricmp(iter->c_str(), "sign") == 0) {
			list<string>::iterator niter = m_listRechargeIP.begin();
			list<string>::iterator niter1 = m_listRechargeSign.begin();
			string curClientIP = inet_ntoa(curClient->m_sockaddr.sin_addr);
			unsigned int i = 0;
			for (; i < m_listRechargeIP.size(); i++, niter++, niter1++) {
				if (*niter == curClientIP) 
					break;
			}
			if (i != m_listRechargeIP.size()) {
				tempMD5Code += *niter1;
				break;
			}
		}
		tempMD5Code += curClient->getClientArg(*iter);
	}

	MD5Operate tempMD5Obj;
	tempMD5Obj.GenerateMD5((unsigned char *)tempMD5Code.c_str(), tempMD5Code.length());
	cout << "Sign:" << curClient->m_mapElement["Sign"] << endl;
	cout << "MD5:" << tempMD5Obj.ToString() << endl;
	if (_stricmp(curClient->m_mapElement["sign"].c_str(), tempMD5Obj.ToString().c_str()) == 0) {
		return false;
	}

	return true;
}


/*
	功能：显示执行结果信息
*/
void ServerConf::showExcResult(int resultCode) {
	switch (resultCode) {
	case 300: {
		OutputDebugString("not fond this command\n");
		printf("not fond this command\n");
	}break;
	case 200: {
		OutputDebugString("PUT command not fond corresponding operation\n");
		printf("PUT command not fond corresponding operation\n");
	}break;
	case 100: {
		OutputDebugString("POST command not fond corresponding operation\n");
		printf("POST command not fond corresponding operation\n");
	}break;
	case 0: {
		OutputDebugString("GET command not fond corresponding operation\n");
		printf("GET command not fond corresponding operation\n");
	}; break;
	case 1: {
		OutputDebugString("GET command GET_ACCOUNT_SUCCESS\n");
		printf("GET command GET_ACCOUNT_SUCCESS\n");
	}; break;
	case 2: {
		OutputDebugString("GET command GET_ACCOUNT_FALSE\n");
		printf("GET command GET_ACCOUNT_FALSE\n");
	}; break;
	case 3: {
		OutputDebugString("GET command GET_EQUIP_SUCCESS\n");
		printf("GET command GET_EQUIP_SUCCESS\n");
	}; break;
	case 4: {
		OutputDebugString("GET command GET_EQUIP_FALSE\n");
		printf("GET command GET_EQUIP_FALSE\n");
	}; break;
	case 5: {
		OutputDebugString("GET command GET_TIMER_SUCCESS\n");
		printf("GET command GET_TIMER_SUCCESS\n");
	}; break;
	case 6: {
		OutputDebugString("GET command GET_TIMER_FALSE\n");
		printf("GET command GET_TIMER_FALSE\n");
	}; break;
	case 7: {
		OutputDebugString("GET command GET_RECHARGE_SUCCESS\n");
		printf("GET command GET_RECHARGE_SUCCESS\n");
	}; break;
	case 8: {
		OutputDebugString("GET command GET_RECHARGE_FALSE\n");
		printf("GET command GET_RECHARGE_FALSE\n");
	}; break;
	case 9: {
		OutputDebugString("GET command GET_FLUSHDATA_SUCCESS\n");
		printf("GET command GET_FLUSHDATA_SUCCESS\n");
	}; break;
	case 10: {
		OutputDebugString("GET command GET_FLUSHDATA_FALSE\n");
		printf("GET command GET_FLUSHDATA_FALSE\n");
	}; break;
	default: {
		OutputDebugString("requistRet error\n");
		printf("requistRet error\n");
	}break;
	}

	return;
}

/*
	功能：装备查询操作，时间戳校验工作
	参数：连接 SOCKET
*/

bool ServerConf::equipCheck(ClientInfo * curClient) {
	if ( !timeInspect(curClient) ) {
		OutputDebugString("道具发送 超时\n");
		printf("道具发送 超时\n");
		return false;
	}
	if (curClient->m_mapElement.find("chaid") == curClient->m_mapElement.end() || curClient->m_mapElement["chaid"] == "" ) {
		OutputDebugString("道具发送 chaid 异常\n");
		printf("道具发送 chaid 异常\n");
		return false;
	}

	string tempStore = "";
	if (runPath == 0 || runPath == 1) {
		tempStore = "exec [CronousDataNew].[dbo].[USP_SAVE_GIFTDATA] ";
	}
	else if (runPath == 2) {
		tempStore = "exec [CronousData].[dbo].[USP_SAVE_GIFTDATA] ";
	}
	else if (runPath == 3) {
		tempStore = "exec [CronousData].[dbo].[USP_SAVE_GIFTDATA] ";
	}
	else {
		OutputDebugString("userinfo runPath 异常");
		printf("userinfo runPath 异常");
		return false;
	}
		
	map<string, string> tempMapArg;
	for (list<string>::iterator iter = m_listEquip.begin(); 
								iter != m_listEquip.end(); 
								iter++ ) {
		string tempName;
		if (   ((*iter).find("itemid") != -1)  
			|| ((*iter).find("itemcount") != -1)
			|| ((*iter).find("itemname") != -1)) {
			for (int i = 1; i <= 5; i++) {
				tempName = *iter + to_string(i);
				if (curClient->m_mapElement.find(tempName) != curClient->m_mapElement.end()) {
					tempMapArg[tempName] = curClient->m_mapElement[tempName];
				}
				else {
					tempMapArg[tempName] = "0";
				}
			}
		}
		else {
			if (curClient->m_mapElement.find(*iter) != curClient->m_mapElement.end()) {
				tempStore += curClient->m_mapElement[*iter] + ",";
			}
			else {
				tempStore += "0,";
			}
		}
	}

	for (int i = 1; i <= 5; i++ ) {
		string tempName;
		tempName = "itemid" + to_string(i);
		if (tempMapArg.find(tempName) == tempMapArg.end()) {
			tempStore += "0,";
		}
		else {
			tempStore += tempMapArg[tempName] + ",";
		}

		tempName = "itemcount" + to_string(i);
		if (tempMapArg.find(tempName) == tempMapArg.end()) {
			tempStore += "0,";
		}
		else {
			tempStore += tempMapArg[tempName] + ",";
		}
	}
	printf( "装备赠送功能：%s\n", tempStore.c_str());
	OutputDebugString("准备查询 存储过程执行失败\n");
	tempStore.erase(tempStore.length()-1, 1);

	if ( m_SQLServer.executeSql(tempStore) ) {
		m_SQLServer.resolveSelectInfo();
		return true;
	}
	else {
		OutputDebugString("准备查询 存储过程执行失败\n");
		printf("准备查询 存储过程执行失败\n");
		return false;
	}
}

/*
	功能：查询玩家在线信息
	可以查询服务器上的当前
		各线的 当前人数、每天、每周、每月 人数的统计
*/
bool ServerConf::timeMoniter(ClientInfo * curClient) {
	list<string>::const_iterator iter;
	for (iter = m_listTimer.begin(); iter != m_listTimer.end(); iter++) {
		if(*iter == "EndTime")
			continue;
		if (*iter == "StartTime")
			continue;
		if (curClient->m_mapElement.find((*iter).c_str()) == curClient->m_mapElement.end()) {
			OutputDebugString("userinfo 标准参数匹配异常\n");
			printf("userinfo 标准参数匹配异常\n");
			return false;
		}
	}
	if (curClient->m_mapElement.find("Moment") == curClient->m_mapElement.end()) {
		OutputDebugString("缺少重要参数 Moment\n");
		printf("缺少重要参数 Moment\n");
		return false;
	}

	int moMent = atoi(curClient->m_mapElement.find("Moment")->second.c_str());
	if (moMent == 0) {//在线查询
		int peopleNum = selectOnGameCharacter(curClient);
		if ( peopleNum == -1) {
			OutputDebugString("当前在线人数查询失败\n");
			printf("当前在线人数查询失败\n");
			return false;
		}
		else{
			curClient->m_succesStr = to_string(peopleNum);
			string str = "当前在线人数    ";
			str += to_string(peopleNum);
			OutputDebugString(str.c_str());
			printf(str.c_str());
			return true;
		}
	}
	else if (moMent == 1 || moMent == 2 || moMent == 3 || moMent == 4) {//历史查询
		int peopleNum = selectPastLog(curClient);
		if ( peopleNum == -1 ) {
			OutputDebugString("历史在线人数查询失败\n");
			printf("历史在线人数查询失败\n");
			return false;
		}
		else {
			curClient->m_succesStr = to_string(peopleNum);
			string str = "历史在线人数    ";
			str += to_string(peopleNum);
			OutputDebugString(str.c_str());
			printf(str.c_str());
			return true;
		}
	}
	else {
		return false;
	}

	//查询当前在线人数为 采用查询本地服务端口查询。时刻查询采用数据库历史查询
	return false;
}

/*
	功能：给指定玩家充值
*/
bool ServerConf::rechargeOperate(ClientInfo * curClient) {
	//1，进行IP效验
	struct sockaddr_in currentAddr;
	int currentLen = sizeof(currentAddr);
	ZeroMemory(&currentAddr, sizeof(currentAddr));
	getpeername(curClient->m_Socket, (struct sockaddr *)&currentAddr, &currentLen);
	curClient->m_sockaddr = currentAddr;

	string curClientIP = inet_ntoa(curClient->m_sockaddr.sin_addr);
	if ( !checkRechargeIP(curClientIP) ) {
		OutputDebugString("充值操作 IP效验 失败\n");
		printf("充值操作 IP效验 失败\n");
		return false;
	}
	
	//2，参数完整性效验
	if ( !rechargeInspect(curClient) ) {
		OutputDebugString("充值操作 数据完整性效验 失败\n");
		printf("充值操作 数据完整性效验 失败\n");
		return false;
	}

	//3，执行存储过程
	string tempSql = "";
	if (runPath == 0 || runPath == 1) {
		tempSql = "exec [Cronous].[dbo].[DeltaCash] ";
	}
	else if (runPath == 2) {
		tempSql = "exec [Data].[Cronous].[dbo].[DeltaCash] ";
	}
	else if (runPath == 3) {
		tempSql = "exec [Cronous].[dbo].[DeltaCash] ";
	}
	for (list<string>::iterator iter = m_listRecharge.begin(); iter != m_listRecharge.end(); iter++) {
		if ( curClient->m_mapElement.find((*iter).c_str()) == curClient->m_mapElement.end()) {
			OutputDebugString("userinfo 标准参数缺少\n");
			printf("userinfo 标准参数缺少\n");
			return false;
		}
		if ( stricmp(iter->c_str(), "sign") == 0 ) {
			continue;
		}
		tempSql += curClient->m_mapElement.find((*iter).c_str())->second + ',';
	}
	tempSql.erase(tempSql.length() - 1, 1);

	if (m_SQLServer.executeSql(tempSql)) {
		m_SQLServer.resolveSelectInfo();
		return true;
	}
	else {
		OutputDebugString("充值操作 执行存储过程 失败\n");
		printf("充值操作 执行存储过程 失败\n");
		return false;
	}

	return true;
}


/* 利用指令刷新游戏服务端的信息。主要刷新 dat 资源文件，开启各种双倍等 */
bool ServerConf::flushServerData(ClientInfo * curClient) {
	bool FuncRet = true;
	if (curClient->m_mapElement.find("Target") == curClient->m_mapElement.end()) {
		printf("刷新服务器数据操作缺少Target数据\n");
		return FuncRet = false;
	}
	if (curClient->m_mapElement.find("Account") == curClient->m_mapElement.end()) {
		printf("刷新服务器数据操作缺少Account数据\n");
		return FuncRet = false;
	}
	if (curClient->m_mapElement.find("Passwd") == curClient->m_mapElement.end()) {
		printf("刷新服务器数据操作缺少Passwd数据\n");
		return FuncRet = false;
	}
	GetPrivateProfileStringA("GMInfo", "Account", "administrator", GMInfo.GAccount, 64, ".\\userinfo.ini");
	GetPrivateProfileStringA("GMInfo", "Passwd", "123", GMInfo.GPasswd, 64, ".\\userinfo.ini");
	GetPrivateProfileStringA("GMInfo", "ClientIp", "127.0.0.1", GMInfo.GIp, 64, ".\\userinfo.ini");
	GMInfo.GPort = GetPrivateProfileIntA("GMInfo", "Port", 2970, ".\\userinfo.ini");
	printf("获取的GMInfo:\n\t Account:%s\n\t Passwd:%s\n\t Ip:%s\n\t Port:%d\n",
		GMInfo.GAccount, GMInfo.GPasswd, GMInfo.GIp, GMInfo.GPort);

	if (stricmp(curClient->m_mapElement["Account"].c_str(), GMInfo.GAccount) != 0) {
		printf("刷新服务器数据账号不匹配\n");
		return FuncRet = false;
	}
	if (stricmp(curClient->m_mapElement["Passwd"].c_str(), GMInfo.GPasswd) != 0) {
		printf("刷新服务器数据密码不匹配\n");
		return FuncRet = false;
	}
	struct sockaddr_in currentAddr;
	int currentLen = sizeof(currentAddr);
	ZeroMemory(&currentAddr, sizeof(currentAddr));
	getpeername(curClient->m_Socket, (struct sockaddr *)&currentAddr, &currentLen);
	curClient->m_sockaddr = currentAddr;
	string curClientIP = inet_ntoa(curClient->m_sockaddr.sin_addr);
	if (stricmp(curClientIP.c_str(), GMInfo.GIp) != 0) {
		printf("非专用IP: %s\n", curClientIP.c_str());
		return FuncRet = false;
	}

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
		printf("套接字库初始化失败\n");
		return FuncRet = false;
	}

	SOCKET serverSC;
	serverSC = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSC == INVALID_SOCKET) {
		printf("套接字初始化失败\n");
		return FuncRet = false;
	}

	struct sockaddr_in serverAddr = { 0 };
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(GMInfo.GPort);
	serverAddr.sin_addr.s_addr = inet_addr(GMInfo.GIp);
	if (connect(serverSC, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
		printf("连接 CronousServer:%s,%d 服务器失败\n", "127.0.0.1", 2970);
		return FuncRet = false;
	}

	int targetNum = atoi(curClient->m_mapElement["Target"].c_str());
	SMAData sd = { 0 };
	SMAMent sm = { 0 };
	SMALogin sl = { 0 };
	sd.header = targetNum;
	sm.header = targetNum;
	sl.header = targetNum;
	char *pSend = (char *)&sd;

	switch (targetNum) {
	case maMent:	strncpy(sm.msg, "massage", 90); pSend = (char *)&sm;	break;
	case maLogin: { strcpy_s(sl.UID, GMInfo.GAccount);
		strcpy_s(sl.passwd, GMInfo.GPasswd);
		pSend = (char *)&sl; };											break;
	case maNotice:				sd.data1 = 10;							break;
	case maNoticeFile:;													break;
	case maIni:					sd.data1 = 10;							break;
	case maIniFile:;													break;
	case maDoubleExp:			sd.PercentData = 10;					break;
	case maDoubleGold:			sd.PercentData = 10;					break;
	case maFixDropGoldrate:		sd.PercentData = 10;					break;
	case maDoubleItem:			sd.PercentData = 10;					break;
	case maFreeInit:			sd.data1 = 10;							break;
	case maAllowPK:				sd.data1 = 10;							break;
	case maScenario:;											break;
	case maReloadScript:;										break;
	case maScenarioData:;										break;
	case maCombineEvent:;										break;
	case maGrowthNameEvent:;									break;
	case maUserLimit:;											break;
	case maRefreshNPC:;											break;
	case maListenMobile:;										break;
	case maRollingNotice:;										break;
	case maBeginnerGuildNotice:;								break;
	default:;													break;
	}

	switch (targetNum) {
	case maMent: {
		send(serverSC, pSend, sizeof(SMAMent), NULL);
		FuncRet = true;
	}; break;
	case maLogin: {
		send(serverSC, pSend, sizeof(SMALogin), NULL);
		char recvBuf[2] = { 0 };
		int ret = recv(serverSC, recvBuf, 1024, NULL);
		FuncRet = !recvBuf[1];
	}; break;
	default: {
		send(serverSC, pSend, sizeof(SMAData), NULL);
		FuncRet = true;
	}; break;
	}

	closesocket(serverSC);
	return FuncRet;
}


/*
	功能：根据端口连接数查询玩家在线人数
	在线人数 = (netstat查询结果数 - 本机端口占用数[1]) / 单向连接[2]
*/
int ServerConf::selectOnGameCharacter(ClientInfo * curClient) {
	string tempPort = "";
	char retBuffer[1024 * 4] = {};
	char cmdBuffer[128] = {};
	if (curClient->m_mapElement.find("Link") == curClient->m_mapElement.end())
		return -1;
	int tempLink = atoi(curClient->m_mapElement.find("Link")->second.c_str());
	if (tempLink < 1)
		return -1;
	list<string>::const_iterator iter = m_listPort.begin();
	while (tempLink > 1) {
		if (iter != m_listPort.end()) {
			iter++;
		}
		tempLink--;
	}
	tempPort = *iter;

	sprintf_s(cmdBuffer, "netstat -an|find \"%s\" /c", tempPort.c_str());
	if (cxecCmd(cmdBuffer, retBuffer, sizeof(retBuffer))) {
		return atoi(retBuffer);
	}
	return -1;
}


/*
	功能：更具用户的Moment参数确认需要在数据库中查询的玩家历史在线数
	Moment==1[参考结束时间查询]
	Moment==2[一天]
	Moment==3[一周]
	Moment==4[一月]
*/
int ServerConf::selectPastLog(ClientInfo * curClient) {
	if (curClient->m_mapElement.find("Moment") == curClient->m_mapElement.end())
		return -1;
	if (curClient->m_mapElement.find("StartTime") == curClient->m_mapElement.end())
		return -1;
	string tempSelect = "";
	if (runPath == 0 || runPath == 1) {
		tempSelect = "[CronousDataNew].[dbo].[USP_SELECT_CHARACTER_ONLINE](";
	}
	else if (runPath == 2) {
		tempSelect = "[Data].[CronousData].[dbo].[USP_SELECT_CHARACTER_ONLINE](";
	}
	else if (runPath == 3) {
		tempSelect = "[CronousData].[dbo].[USP_SELECT_CHARACTER_ONLINE](";
	}
	else
		return -1;
	
	int moMent = atoi(curClient->m_mapElement.find("Moment")->second.c_str());
	switch (moMent) {
	case 1: {
		if (curClient->m_mapElement.find("EndTime") == curClient->m_mapElement.end())
			return -1;
		tempSelect += curClient->m_mapElement.find("Moment")->second + ",\'";
		tempSelect += curClient->m_mapElement.find("StartTime")->second + "\',\'";
		tempSelect += curClient->m_mapElement.find("EndTime")->second + "\')";
	}
	break;
	case 2:
	case 3:
	case 4: {
		tempSelect += curClient->m_mapElement.find("Moment")->second + ",\'";
		tempSelect += curClient->m_mapElement.find("StartTime")->second + "\')";
	}
	break;
	default: 
		return -1;
	}

	if (m_SQLServer.executeStore(tempSelect)) {
		string tempResult = "";
		m_SQLServer.resolveSelectInfo();
		tempResult = JsonOperate::multimapParseJson(m_SQLServer.getcurSelectInfo());
		curClient->m_succesStr = tempResult;
		return atoi(m_SQLServer.getcurSelectInfo().begin()->second.c_str());
	}
	else {
		return -1;
	}
}


// 描述:execmd函数执行命令，并将结果存储到result字符串数组中 
// 参数:cmd表示要执行的命令
// result是执行的结果存储的字符串数组
bool ServerConf::cxecCmd(const char* cmd, char* pBuffer, unsigned int bufferLen) {
	char buffer[128];
	FILE* pipe = _popen(cmd, "r");            //打开管道，并执行命令 
	if (!pipe)
		return false;                      //返回0表示运行失败 

	while (!feof(pipe)) {//查看输出流，如果输出流有数据，则存储与result
		if (fgets(buffer, 128, pipe)) { 
			strcat_s(pBuffer, bufferLen, buffer);
		}
	}
	_pclose(pipe);
	return true;
}


/*
	功能：用户账户查询工作，利用账户信息或者角色信息查询出对应的信息
*/
bool ServerConf::accountInfo(ClientInfo * curClient) {
	OutputDebugString("accountInfo begin");
	if ( !elementInspect(curClient) ) {
		OutputDebugString("角色查询 参数效验异常\n");
		printf("角色查询 参数效验异常\n");
		return false;
	}
	OutputDebugString("postpoint2\n");
	if (curClient->m_mapElement.find("AccountName") != curClient->m_mapElement.end() && curClient->m_mapElement["AccountName"] != "") {
		string tempAccount = curClient->m_mapElement["AccountName"];
		string tempName = "\'\'";
		string tempActive = "2";

		if (curClient->m_mapElement.find("Active") == curClient->m_mapElement.end()) {
			tempActive = "2";
		}
		else {
			tempActive = curClient->m_mapElement["Active"];
			if (tempActive != "1" && tempActive != "2")
				tempActive = "1";
		}

		string tempSql = "";
		if (runPath == 0 || runPath == 1) {
			tempSql = "exec [CronousDataNew].[dbo].[USP_SELECT_ACCOUNTINFO] ";
		}
		else if (runPath == 2) {
			tempSql = "exec [CronousData].[dbo].[USP_SELECT_ACCOUNTINFO] ";
		}
		else if (runPath == 3) {
			tempSql = "exec [CronousData].[dbo].[USP_SELECT_ACCOUNTINFO] ";
		}
		else
			return false;
		tempSql += tempAccount + ",";
		tempSql += tempActive + ",";
		tempSql += tempName;
		if ( m_SQLServer.executeSql(tempSql) ) {
			string tempResult = "";
			m_SQLServer.resolveSelectInfo();
			tempResult = JsonOperate::multimapParseJson( m_SQLServer.getcurSelectInfo() );
			curClient->m_succesStr = tempResult;
			return true;
		}
		return false;
	}
	else if (curClient->m_mapElement.find("chName") != curClient->m_mapElement.end() && curClient->m_mapElement["chName"] != "") {
		string tempAccount = "\'\'";
		string tempName = curClient->m_mapElement["chName"];
		int Active = 2;
		string tempActive = "2";
		if (curClient->m_mapElement.find("Active") == curClient->m_mapElement.end()) {
			tempActive = "2";
		}
		else {
			tempActive = curClient->m_mapElement["Active"];
			if (tempActive != "1" && tempActive != "2")
				tempActive = "1";
		}
		string tempSql = "";
		if (runPath == 0 || runPath == 1) {
			tempSql = "exec [CronousDataNew].[dbo].[USP_SELECT_ACCOUNTINFO] ";
		}
		else if (runPath == 2) {
			tempSql = "exec [CronousData].[dbo].[USP_SELECT_ACCOUNTINFO] ";
		}
		else if (runPath == 3) {
			tempSql = "exec [CronousData].[dbo].[USP_SELECT_ACCOUNTINFO] ";
		}
		else
			return false;
		tempSql += tempAccount + ",";
		tempSql += tempActive + ",";
		tempSql += tempName;
		if (m_SQLServer.executeSql(tempSql)) {
			string tempResult = "";
			m_SQLServer.resolveSelectInfo();
			tempResult = JsonOperate::multimapParseJson(m_SQLServer.getcurSelectInfo());
			curClient->m_succesStr = tempResult;
			return true;
		}
		return false;
	}
	else {
		OutputDebugString("角色参数 AccountName 或 ");
		printf("角色参数 AccountName 或 ");
		return false;
	}
	return false;
}


/*
	功能：判断当前的客户socket时间是否过期
	参数：客户socket
*/
bool ServerConf::timeInspect(ClientInfo * curClient) {
	string tempSNew = to_string(  time(NULL) );
	string tempSOld = to_string( curClient->m_SelectTime );
	if (tempSOld == "") {
		return true;
	}
	
	long timeNEW = stol(tempSNew);
	long timeOLD = stol(tempSOld);

	if ( (timeNEW-timeOLD) < 300000 ) {
		return true;
	}

	return true;
}



/*
	功能：服务端存储客户的链接。目的是让查询工作和发送道具工作的数据保存
*/
ClientInfo * ServerConf::saveClientLink(ClientInfo * argClient) {
	OutputDebugString("saveClientLink begin");
	struct sockaddr_in currentAddr;
	int currentLen = sizeof(currentAddr);
	ZeroMemory(&currentAddr, sizeof(currentAddr));
	getpeername(argClient->m_Socket, (struct sockaddr *)&currentAddr, &currentLen);
	int i = 0;
	for (; i < MAXCLIENTSIZE; i++) {
		MainServer->m_Client[i].m_Timer += 1;
		if (MainServer->m_Client[i].m_sockaddr.sin_addr.s_addr == currentAddr.sin_addr.s_addr) {
			argClient->m_Timer = MainServer->m_Client[i].m_Timer;
			argClient->m_SelectTime = MainServer->m_Client[i].m_SelectTime;
			MainServer->m_Client[i].ClientInfoInit();
			MainServer->m_Client[i] = *argClient;
			MainServer->m_Client[i].m_sockaddr = currentAddr;
			argClient = &(MainServer->m_Client[i]);
			break;
		}
		if (MainServer->m_Client[i].m_Socket == 0) {
			argClient->m_Timer = MainServer->m_Client[i].m_Timer;
			MainServer->m_Client[i].ClientInfoInit();
			MainServer->m_Client[i] = *argClient;
			MainServer->m_Client[i].m_sockaddr = currentAddr;
			argClient = &(MainServer->m_Client[i]);
			break;
		}
	}
	if (i == MAXCLIENTSIZE) {				//换出最久的客户存储
		int maxClient = 0;
		int maxTimer = 0;
		for (i = 0; i < MAXCLIENTSIZE; i++) {
			if (MainServer->m_Client[i].m_Timer >= (unsigned int)maxTimer) {
				maxClient = i;
				maxTimer = MainServer->m_Client[i].m_Timer;
			}
		}
		MainServer->m_Client[maxClient].ClientInfoInit();
		MainServer->m_Client[maxClient] = *argClient;
		MainServer->m_Client[maxClient].m_sockaddr = currentAddr;
		argClient = &(MainServer->m_Client[maxClient]);
	}

	OutputDebugString("saveClientLink end");
	return argClient;
}

/*
	功能：线程执行函数。当有效用户进入后这是需要执行的操作
*/
UINT _stdcall threadAccept_request(LPVOID argLP) {
	OutputDebugString("threadAccept_request begin!!!\n");
	unsigned int tempClientSock = *((unsigned int *)argLP);

	//提取 URL 请求字符串
	char buf[1024] = "";
	int tempLen = MainServer->getSock_line(tempClientSock, buf, sizeof(buf));
	if ( tempLen == -1 ) {
		closesocket(tempClientSock);
		return 0;
	}
	string tempClientStr = buf;
	if (tempClientStr.find("favicon.ico") != -1) {
		closesocket(tempClientSock);
		return 0;
	}

	//客户端分解参数
	ClientInfo tempClient;
	strcpy_s(tempClient.m_Buffer, buf);
	tempClient.m_Socket = tempClientSock;
	if (!tempClient.elementResolve()) {
		tempClient.requistFalse();
		shutdown(tempClientSock, 1);
		closesocket(tempClientSock);
		OutputDebugString("elementResolve error \n");
		printf("elementResolve error \n");
		closesocket(tempClientSock);
		return -1;//结束
	}
	else {
		OutputDebugString("elementResolve success \n");
		printf("elementResolve success \n");
	}
	
	//执行请求并显示执行结果
	int requistRet = MainServer->requistExecute(&tempClient);
	OutputDebugString("postpoint99\n");
	MainServer->showExcResult(requistRet);
	OutputDebugString("postpoint98\n");
	tempClient.discardheaders();

	shutdown(tempClientSock, 1);
	closesocket(tempClientSock);
	OutputDebugString("threadAccept_request end!!!\n");
	return 0;
}