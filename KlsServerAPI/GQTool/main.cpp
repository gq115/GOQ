#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include "JsonOperate.h"
#include "MD5Operate.h"
#include "SQLserverOperate.h"
#include "ServerConf.h"

#define MAXMANAGER 128

using namespace std;

ServerConf *MainServer;

int main() {
	
	MainServer = new(ServerConf);
	MainServer->initServer( 9090 );

	MainServer->acceptManage();

	delete MainServer;
	
	Sleep(10000);
	return 0;
}