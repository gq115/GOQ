import pymssql
import threading
import config
from log import LOG


DBLock = threading.Lock()

# 定义数据库管理对象
class DBUnit:
    #对象初始化，负责数据库的连接
    def __init__(self, ip=None, db=None, user=None, passw=None):
        try:
            self.user = str(user)
            self.passw = str(passw)
            self.ip = str(ip)
            self.db = str(db)
            self.connect = pymssql.connect(host=self.ip, user=self.user, password=self.passw, database=self.db)
            self.connect.autocommit(True)
        except Exception as a:
            msg = "Could not connect to DBServer of {0}:{1}".format(self.ip, self.db)
            self.connect = False
            LOG.error(msg)
        else:
            msg = "DBServer connect:host:{0}, User:{1}, Pass:{2}, DB:{3}".format(self.ip, self.user, self.passw, self.db)
            LOG.info(msg)
    #-----------------------------------------------------------------------------

    #重新建立数据库连接
    def reconnect(self):
        LOG.info("begin reconnect")
        try:
            if self.connect == True:
                return True
            with DBLock:
                self.connect = pymssql.connect(host=self.ip, user=self.user, password=self.passw, database=self.db)
                if self.connect == True:
                    self.connect.autocommit(True)
                    LOG.info("reconnect success")
                    return True
        except Exception as e:
            self.connect = False
            LOG.error("reconnect False")
            return False
        self.connect = False
        LOG.warning("reconnect False")
    #-----------------------------------------------------------------------------

    #检查数据库的链接，并查看是否可用
    def checkconnect(self):
        LOG.info("begin checkConnect")
        if self.connect == False:
            LOG.info("checkconnect result is False")
            return False
        try:
            sql = "select 1 as go"
            cursor = self.connect.cursor()
            cursor.execute()
            cursor.close()
        except Exception as E:
            self.connect = False
            LOG.error("connect execute Error")
            return False
        else:
            LOG.info("checkconnect resul is True")
            return True
    #-----------------------------------------------------------------------------

    #执行一个存储过程
    def exec_proc(self, procStr):
        if self.connect == False:
            if self.reconnect() == False:
                resultStr = "exec_proc reconnect False"
                LOG.error(resultStr)
                return False,resultStr
            LOG.info("exec_Proc connect is False, but reconnect is Ture")
        try:
            with DBLock:
                cursor = self.connect.cursor()
                LOG.info("exec_proc str:{0}".format(procStr))
                cursor.execute(procStr)
                resultStr = cursor.fetchall()
                resultCount = len(resultStr)
                if resultCount >= 10000:
                    LOG.warning("result count oversize:{0}".format(resultCount))
                    cursor.close()
                    return False
                cursor.close()
                return True, resultStr
        except Exception as E:
            resultStr = str(E)
            LOG.error("exec_proc Error:{0}".format(resultStr))
            return False,resultStr
    #-----------------------------------------------------------------------------

    #执行一个sql语句
    def exec_sql(self, sqlStr):
        if self.connect == False:
            if self.reconnect() == False:
                resultStr = "exec_sql reconnect False"
                LOG.error(resultStr)
                return False,resultStr
            LOG.info("exec_Proc connect is False, but reconnect is Ture")
        try:
            with DBLock:
                cursor = self.connect.cursor()
                LOG.info("exec_sql str:{0}".format(sqlStr))
                cursor.execute(sqlStr)
                resultStr = cursor.fetchall()
                resultCount = len(resultStr)
                if resultCount >= 10000:
                    LOG.warning("result count oversize:{0}".format(resultCount))
                    cursor.close()
                    return False
                cursor.close()
                return True, resultStr
        except Exception as E:
            resultStr = str(E)
            LOG.error("exec_sql Error:{0}".format(resultStr))
            return False,resultStr
    #-----------------------------------------------------------------------------

if __name__ == "__main__":
    DBObj = DBUnit( config.DBINFO["ip"], config.DBINFO["DB"], config.DBINFO["User"], config.DBINFO["Pass"] )
    sqlStr = "SELECT * FROM [DR2_User].[dbo].[TB_CharacterBasic] where Name='DJ喜洋洋'"
    ret, resultStr = DBObj.exec_sql(sqlStr)
    print(ret, resultStr)
    procStr = "exec DR2_User.dbo.up_wingmGetUserItem \'743D0CE3-5293-4844-B357-8065667BC1B2\'"
    ret, resultStr = DBObj.exec_proc(procStr)
    print(ret, resultStr)