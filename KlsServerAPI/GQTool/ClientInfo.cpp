#include "ClientInfo.h"


using namespace std;


ClientInfo::ClientInfo() {
	m_SelectTime = 0;
	ClientInfoInit();
}

ClientInfo::~ClientInfo() {
	m_SelectTime = 0;
	ClientInfoInit();
}

void ClientInfo::ClientInfoInit() {
	m_Type = 0;
	m_Socket = 0;
	m_Timer = 0;
	m_strMethod = "";
	m_strPath = "\\";
	m_succesStr = "";
	m_errorStr = "";
	ZeroMemory(&m_sockaddr, sizeof(sockaddr_in));
	ZeroMemory(m_Buffer, MAXBUFFERSIZE);
	//m_mapElement.clear();
}

/*
	功能：分解http请求头，并将请求方式和请求参数存储于程序中
	参数：http请求头
*/
bool ClientInfo::elementResolve() {
	OutputDebugString("elementResolve begin!!!\n");
	printf("elementResolve begin!!!\n");
	string argUrl = m_Buffer;
	if ((argUrl.length() == 0) && (argUrl == "")) {
		OutputDebugString("elementResolve end!!!\n");
		printf("elementResolve end!!!\n");
		return false;
	}

	string switchStr;
	switchStr = strCoding::UrlUTF8Decode(argUrl);
	cout << "正常的 Url 请求头:" << argUrl << endl;
	cout << "转换后的 Url 请求头:" << switchStr << endl;
	if (switchStr.find_last_of('?') != switchStr.length() - 1)		//编码不一可能导致程序崩溃
		;//argUrl = switchStr;
	if (argUrl.find('%') != -1) {
		argUrl = switchStr;
	}

	//去除 Url 中的首尾空格
	int i = argUrl.find_first_not_of(" ");
	int j = argUrl.find_last_not_of(" ");
	if ((i != 0) && (i != -1)) {
		argUrl.erase(0, i);
	}
	if ( (j != (argUrl.length() - 1)) && (j != -1)) {
		argUrl.erase(j + 1);
	}
	//去除协议类型
	if ( (argUrl.find_last_of(" ") == -1) || (argUrl.find_last_of(" ") == argUrl.find_first_of(" "))) {
		return false;
	}
	argUrl.erase(argUrl.find_last_of(" "));

	//获取请求方式
	i = argUrl.find_first_of(" ");
	m_strMethod = argUrl.substr(0, i);

	j = argUrl.find_first_of("/");
	i = argUrl.find("?");
	if (i == -1 || j == -1) {
		return false;
	}
	/*if (i == -1) {
		m_strPath = argUrl.substr(j - 1, -1);
		OutputDebugString("elementResolve end!!!\n");
		printf("elementResolve end!!!\n");
		return true;
	}*/
	else {
		m_strPath = argUrl.substr(j, i - j);
		if (i == argUrl.length() - 1) {
			OutputDebugString("elementResolve end!!!\n");
			printf("elementResolve end!!!\n");
			return true;
		}
	}

	string tempEle = argUrl.substr(i + 1, -1);
	string tempEleCo = "";
	string tempName = "", tempValue = "";
	m_mapElement.clear();
	do {
		j = tempEle.find("&");
		if (j == -1) {
			tempEleCo = tempEle;
			if (tempEleCo.find("=") != -1) {
				tempName = tempEleCo.substr(0, tempEleCo.find("="));
				tempValue = tempEleCo.substr(tempEleCo.find("=") + 1, -1);
				m_mapElement.insert(pair<string, string>(tempName, tempValue));
			}
			break;
		}
		else {
			tempEleCo = tempEle.substr(0, j);
			tempEle = tempEle.substr(j + 1, -1);
			if (tempEleCo.find("=") == -1) {
				i = j + 1;
				continue;
			}
			else {
				tempName = tempEleCo.substr(0, tempEleCo.find("="));
				tempValue = tempEleCo.substr(tempEleCo.find("=") + 1, -1);
				m_mapElement.insert(pair<string, string>(tempName, tempValue));
			}
		}
	} while (1);
	
	map<string, string>::iterator iter;
	iter = m_mapElement.find("Type");
	if (iter == m_mapElement.end()) {
		OutputDebugString("elementResolve end!!!\n");
		printf("elementResolve end!!!\n");
		return false;
	}
	else
		m_Type = atoi(iter->second.c_str());

	OutputDebugString("elementResolve end!!!\n");
	printf("elementResolve end!!!\n");
	return true;
}

/*
	功能：获取此用户的某个参数的值
*/
string ClientInfo::getClientArg(string argArg) {
	OutputDebugString("getClientArg begin");
	map<string, string>::const_iterator iter = m_mapElement.find(argArg);
	if (iter == m_mapElement.end())
		return "";
	else
		return iter->second;
}


/*
	功能：获取指定 socket 中的一行信息
	参数：指定socket，存储字符串，最大读取字节
	返回值：成功读取字节
*/
int ClientInfo::getSock_line(SOCKET argSock, char * argBuf, int argSize) {
	int i = 0;
	char c = '\0';
	int n;

	while ((i < argSize - 1) && (c != '\n')) {
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

	return(i);
}


/*
	功能：将客户的socket套接字中的所有数据全部读取出。即为清空此套接字
	参数：客户socket套接字
*/

void ClientInfo::discardheaders() {
	char buf[1024];
	int numchars = 1;
	while ((numchars > 0) && strcmp("\n", buf))  /* read & discard headers */
		numchars = getSock_line(m_Socket, buf, sizeof(buf));
}


/*
	功能：返回请求未找到的 http 信息
	参数：指定 socket
*/
void ClientInfo::unimplemented() {
	char buf[1024];
	sprintf_s(buf, 1024, "HTTP/1.0 501 Method Not Implemented\r\n");
	send(m_Socket, buf, strlen(buf), 0);
	sprintf_s(buf, 1024, "Server: bdsoftmgr httpd/0.1.0\r\n");
	send(m_Socket, buf, strlen(buf), 0);
	sprintf_s(buf, 1024, "Content-Type: text/html\r\n");
	send(m_Socket, buf, strlen(buf), 0);
	sprintf_s(buf, 1024, "\r\n");
	sprintf_s(buf, 1024, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
	send(m_Socket, buf, strlen(buf), 0);
	sprintf_s(buf, 1024, "</TITLE></HEAD>\r\n");
	send(m_Socket, buf, strlen(buf), 0);
	sprintf_s(buf, 1024, "<BODY><P>HTTP request method not supported.\r\n");
	send(m_Socket, buf, strlen(buf), 0);
	sprintf_s(buf, 1024, "</BODY></HTML>\r\n");
	send(m_Socket, buf, strlen(buf), 0);
}



/*
	功能：返回请求执行成功的 http 信息
	参数：指定 socket
*/
void ClientInfo::requistSuccess() {
	char buf[1024];
	strcpy_s(buf, 1024, "HTTP/1.0 200 OK\r\n");
	send(m_Socket, buf, strlen(buf), 0);
	strcpy_s(buf, 1024, "bdsoftmgr httpd/0.1.0\r\n");
	send(m_Socket, buf, strlen(buf), 0);
	strcpy_s(buf, 1024, "Content-Type: text/html\r\n");
	send(m_Socket, buf, strlen(buf), 0);
	strcpy_s(buf, 1024, "\r\n");
	send(m_Socket, buf, strlen(buf), 0);

	int tempLen = m_succesStr.length();
	int tempSend = 0;
	while (tempLen > 0) {
		tempSend = send(m_Socket, m_succesStr.c_str(), tempLen, 0);
		m_succesStr.erase(0, tempSend);
		tempLen -= tempSend;
	}
}


/*
	功能：返回请求执行失败的 http 信息
	参数：指定 socket
*/
void ClientInfo::requistFalse() {
	char buf[1024];
	strcpy_s(buf, 1024, "HTTP/1.0 200 OK\r\n");
	send(m_Socket, buf, strlen(buf), 0);
	strcpy_s(buf, 1024, "bdsoftmgr httpd/0.1.0\r\n");
	send(m_Socket, buf, strlen(buf), 0);
	strcpy_s(buf, 1024, "Content-Type: text/html\r\n");
	send(m_Socket, buf, strlen(buf), 0);
	strcpy_s(buf, 1024, "\r\n");
	send(m_Socket, buf, strlen(buf), 0);

	int tempLen = m_errorStr.length();
	int tempSend = 0;
	while (tempLen > 0) {
		tempSend = send(m_Socket, m_errorStr.c_str(), tempLen, 0);
		m_errorStr.erase(0, tempSend);
		tempLen -= tempSend;
	}
}

ClientInfo & ClientInfo::operator=(const ClientInfo & arg) {
	this->m_succesStr = arg.m_succesStr;
	this->m_errorStr = arg.m_errorStr;
	this->m_mapElement = arg.m_mapElement;
	this->m_strPath = arg.m_strPath;
	this->m_strMethod = arg.m_strMethod;
	this->m_Type = arg.m_Type;

	strcpy_s(this->m_Buffer, this->m_Buffer);
	this->m_sockaddr = arg.m_sockaddr;
	this->m_SelectTime = arg.m_SelectTime;
	this->m_Socket = arg.m_Socket;
	this->m_Timer = arg.m_Timer;

	return *this;
}