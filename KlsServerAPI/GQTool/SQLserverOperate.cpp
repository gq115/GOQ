#include "SQLserverOperate.h"
using namespace std;



SQLserverOperate::SQLserverOperate()
{
	m_qConnect = NULL;
	m_qCommand = NULL;
	m_qRecordset = NULL;
	m_accountInfo.clear();
	m_ErrorStr = "no Error";
}


SQLserverOperate::~SQLserverOperate()
{
	if (m_qConnect != NULL) {
		//m_qConnect->Close();
	}
	if (m_qRecordset != NULL) {
		//m_qRecordset->Close();
	}	
}


/*
	功能：
	参数：数据库服务器的IP.Port，数据库名，用户名，用户密码
*/
bool SQLserverOperate::initSQLServer(const char * argIp, const char * argDbName,
	const char * argUser, const char * argPassword) {

	::CoInitialize(NULL);
	HRESULT hr = m_qConnect.CreateInstance( __uuidof(Connection) );
	if ( FAILED(hr) ) {
		m_ErrorStr = "连接对象指针实例化失败. _ConnectPtr::CreateInstance Error";
		return false;
	}

	hr = m_qRecordset.CreateInstance( __uuidof(Recordset) );
	if (FAILED(hr)) {
		m_ErrorStr = "收集对象指针实例化失败. _RecordsetPtr::CreateInstance Error";
		return false;
	}

	hr = m_qCommand.CreateInstance( __uuidof(Command) );
	if (FAILED(hr)) {
		m_ErrorStr = "执行对象指针实例化失败. _CommandPtr::CreateInstance Error";
		return false;
	}

	string modelStr = "Provider=SQLOLEDB;Data Source=%s;User ID=%s;Password=%s;Initial Catalog=%s";
	char tempSqlStr[512] = "";
	try {
		if ( (argIp == NULL) || (argDbName == NULL) || (argUser == NULL) || (argPassword == NULL) ) {
			throw exception("");
		}
		sprintf_s(tempSqlStr, modelStr.c_str(), argIp, argUser, argPassword, argDbName);
	}
	catch (exception) {
		m_ErrorStr = "SQLserverOperate::initSQLServer 字符串处理异常. strcopy Error";
		return false;
	}
	
	try {
		m_qConnect->Open(tempSqlStr, "", "", adModeUnknown );
	}
	catch (_com_error) {
		m_ErrorStr = "SQLserverOperate::initSQLServer Succeed 数据库初始化失败!!!. _ConnectPtr::Open Error";
		m_ErrorStr += "    连接数据库为:";
		m_ErrorStr += tempSqlStr;
		return false;
	}

	cout << "SQLserverOperate::initSQLServer Succeed 数据库初始化成功!!!" << endl;
	return true;
}


/*
	功能：根据单值参数值查询单个列信息
	参数：数据库列名， 数据库表明
*/
bool SQLserverOperate::selectAcountInfo(const char * argColName, const char * argTable, const char * argWhere) {

	char tempSqlModel[] = "SELECT %s FROM %s";
	char tempSqlStr[1024] = "";
	try {
		if ((argColName == NULL) || (argTable == NULL) || (argWhere == NULL)) {
			throw exception("");
		}

		sprintf_s(tempSqlStr, tempSqlModel, argColName, argTable);
		if (argWhere != "") {
			strcat_s( tempSqlStr, " where " );
			strcat_s( tempSqlStr, argWhere );
		}
		strcat_s(tempSqlStr, ";");
	}
	catch (exception) {
		m_ErrorStr = "SQLserverOperate::selectAcountInfo 字符串处理异常. strcpy Error";
		return false;
	}

	try {
		m_qRecordset->Open((_variant_t)tempSqlStr, m_qConnect.GetInterfacePtr(),
			adOpenDynamic, adLockOptimistic, adCmdText);
	}
	catch (_com_error) {
		m_ErrorStr = "数据库查询异常 . _Recordset::Open Error";
		return false;
	}
	
	return true;
}

/*
	功能：根据参数列表值查询多个列信息
	参数：列表，表名
*/
bool SQLserverOperate::selectAcountInfo( const list<string> & argColList, const char * argTable, const char * argWhere) {
	string tempSqlStr = "select ";
	try {
		if ( argColList.empty() || (argTable == NULL) || (argWhere == NULL) || (argTable=="") ) {
			throw exception("");
		}
	}
	catch (exception) {
		m_ErrorStr = "SQLserverOperate::selectAcountInfo . 查询参数异常 ::element Error";
		return false;
	}
	
	typedef list<string>::const_iterator TT;
	TT pos1 = argColList.end();
	pos1--;
	for ( TT pos = argColList.begin(); pos != argColList.end(); pos++ ) {
		tempSqlStr += *pos;
		if ( *pos != *pos1 ) {
			tempSqlStr += ",";
		}
		else {
			tempSqlStr += " from ";
		}
	}
	tempSqlStr += argTable;
	if (argWhere != "") {
		tempSqlStr += " ";
		tempSqlStr += argWhere;
	}
	tempSqlStr += ";";

	try {
		m_qRecordset->Open((_variant_t)tempSqlStr.c_str(), m_qConnect.GetInterfacePtr(),
			adOpenDynamic, adLockOptimistic, adCmdText);
	}
	catch (_com_error) {
		m_ErrorStr = "数据库查询异常 . _Recordset::Open Error";
		return false;
	}

	return true;
}


/*
	功能：利用组建好的数据库操作字符串操作数据库
	参数：标准数据库操作字符串
*/
bool SQLserverOperate::selectAcountInfo(const char * argSqlStr) {
	if ( (argSqlStr == NULL) || (argSqlStr == "") ) {
		m_ErrorStr = "SQLserverOperate::selectAcountInfo 传入参数异常. element Error";
		return false;
	}

	try {
		m_qRecordset->Open((_variant_t)argSqlStr, m_qConnect.GetInterfacePtr(),
			adOpenDynamic, adLockOptimistic, adCmdText);
	}
	catch (_com_error) {
		m_ErrorStr = "SQLserverOperate::selectAcountInfo 传入参数异常. element Error";
		return false;
	}

	return true;
}


/*
	功能：执行指定的存储过程
	参数：执行过程字符串
*/
bool SQLserverOperate::executeStore(string argStoStr) {
	try {
		_variant_t ra;
		m_qRecordset = m_qConnect->Execute( argStoStr.c_str(), &ra, adCmdStoredProc);
	}
	catch (_com_error) {
		m_ErrorStr = "SQLserverOperate::executeStore 执行过程打开失败 Error";
		return false;
	}
	return true;
}

bool SQLserverOperate::executeSql(string argStoStr) {
	try {
		_variant_t ra;
		m_qRecordset = m_qConnect->Execute(argStoStr.c_str(), &ra, adCmdText);
	}
	catch (_com_error) {
		m_ErrorStr = "SQLserverOperate::executeStore 执行过程打开失败 Error";
		return false;
	}
	return true;
}

/*
	功能：按照参数列表处理查询结构，将查询结果分解成有效数据
	参数：无
*/
void SQLserverOperate::resolveSelectInfo() {
	try {
		if (m_qRecordset->adoEOF != VARIANT_FALSE) {
			m_ErrorStr = " 收集对象为空. _RecordsetPtr::adoEOF Error";
			return;
		}
	}
	catch (_com_error) {
		return;
	}
	

	long tempPost = 0;
	string tempColName = "";
	string tempColValue = "";
	long tempRow = m_qRecordset->GetRecordCount();
	long tempCol = m_qRecordset->Fields->Count;
	m_selectResult.clear();

	try {
		//m_qRecordset->MoveFirst();
		while (m_qRecordset->adoEOF == VARIANT_FALSE) {
			for (tempPost = 0; tempPost < tempCol; tempPost++) {
				tempColName = m_qRecordset->Fields->GetItem(tempPost)->Name;
				if (m_qRecordset->Fields->GetItem(tempColName.c_str())->Value.vt == VT_NULL) {
					tempColValue = "";
				}
				else {
					tempColValue = (const char *)(_bstr_t)(m_qRecordset->Fields->GetItem(tempColName.c_str())->Value);
				}
				m_selectResult.insert(pair<string, string>(tempColName, tempColValue));
			}
			m_qRecordset->MoveNext();
		}
	}
	catch (_com_error) {
		m_ErrorStr = "SQLserverOperate::resolveSelectInfo 查询结果分解失败 Error";
	}

	return;
}


/*
	功能：修改指定表中的指定数据
	参数：map(列名，列值)，表明，条件
*/
bool SQLserverOperate::alterRoleInfo(const map<string, string> & argAlterList, const char * argTable, const char * argWhere) {
	string tempSqlStr = "update ";
	if ( (argAlterList.empty()) || (argTable == NULL) || (argWhere == NULL) || (argTable == "") || (argWhere == "") ) {
		m_ErrorStr = "SQLserverOperate::alterRoleInfo  参数异常 element Error ";
		return false;
	}
	tempSqlStr += argTable;
	tempSqlStr += " set ";

	typedef map<string, string>::const_iterator TT;
	TT pos1 = argAlterList.end();
	pos1--;
	for (TT pos = argAlterList.begin(); pos != argAlterList.end(); pos++) {
		if ( pos->first == "" ) {
			continue;
		}
		tempSqlStr += pos->first;
		tempSqlStr += "=";
		tempSqlStr += pos->second;
		if (pos->first == pos1->first) {
			tempSqlStr += " ";
		}
		else {
			tempSqlStr += ",";
		}
	}
	if (argWhere != "") {
		tempSqlStr += "where ";
		tempSqlStr += argWhere;
	}
	tempSqlStr += ";";
	

	try {
		m_qCommand->ActiveConnection = m_qConnect;
		m_qCommand->CommandText = _bstr_t( tempSqlStr.c_str() );
		_variant_t tempAlterNum;
		HRESULT hr = m_qCommand->Execute(&tempAlterNum, NULL, adCmdText);
		if ( !SUCCEEDED(hr) ) {
			m_ErrorStr = "数据库修改失败 _Command::Execute Error ";
			return false;
		}
	}
	catch (_com_error & e) {
		m_ErrorStr = "数据库修改失败 _Command::Execute Error ";
		m_ErrorStr += (char *)e.ErrorMessage();
		return false;
	}
	
	return true;
}