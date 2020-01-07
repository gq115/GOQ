#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import sys
import pymssql
import traceback
import datetime
import _mssql
# print(_mssql.__version__)
import socket
import uuid
import decimal
# print(decimal.__version__)

import time

from mylog import LOGGER

from threading import Lock
lock = Lock()

#保存 当断开连接的时候 执行失败的语句
DB_ERROR_MSG = 'can not connect database'

class DBUnit:
    def __init__(self,user=None,passwd=None,host=None,database=None):
        try:            
            # print('connect database'+database)
            self.host=host
            self.user = user
            self.password = passwd
            self.database = database
            self.connection = pymssql.connect(host=self.host, user = self.user, password =self.password, database=self.database)
            self.connection.autocommit(True)
        except Exception as e:
            # print (e)
            msg = "Could not connect to DB server."
            # print (msg)
            self.connection =False
            LOGGER.info('connect db error {0} '.format(e))
            
    def start_reconnect(self):
        # print('begin start_reconnect')
        LOGGER.info( 'begin start_reconnect')
        try:            
            with lock:
                self.connection = pymssql.connect(host=self.host, user = self.user, password =self.password, database=self.database)
                if self.connection:
                    self.connection.autocommit(True)
                    LOGGER.info( 'reconnect success')
                    return True

        except Exception as e:
            # print (e)
            msg = "Could not connect to DB server."
            # print (msg)
            self.connection =False
            LOGGER.info( 'reconnect failure {0}'.format(str(e)))

            
    def conn(self):
        return self.connection 
    def get_conn(self):
        return self.connection

        
    def close(self):
        self.connection.close()
        self.connection = False
        
    def __del__(self):
        if self.connection != False :
            self.connection.close()


    #检测当前连接是否还是
    def check_is_connected(self):
        if not self.connection:
            return False
        try:
            sql = 'select 1 go'
            cursor = self.connection.cursor()
            cursor.execute(sql)
            result = cursor.fetchone()
            #self.connection.commit()
            cursor.close()
            
        except Exception as e:
            # print(str(e))
            return False

        return True

    def query_proc(self,sql):
        if self.connection == False :
            #执行之前如果当前连接断开 尝试重连
            if not self.start_reconnect():
                return  False,DB_ERROR_MSG

        try:
            with lock:
                cursor = self.connection.cursor()
                LOGGER.info('exec sql ' +sql)
                cursor.execute(sql)
                result = cursor.fetchall()
                cursor.close()
                return True,result 
        except Exception as e:
            result = str(e)
            LOGGER.error('exec_proc_get_one_result catch error:{0}'.format(result))

            return False,result



        pass

    #直接执行存储过程,不返回结果
    def exec_proc(self,sql):
        if self.connection == False :
            #执行之前如果当前连接断开 尝试重连
            if not self.start_reconnect():
                return  False
        try:
            with lock:
                cursor = self.connection.cursor()
                LOGGER.info('exec sql ' +sql)
                cursor.execute(sql)
                cursor.close()
                return True
        except Exception as e:
            result = str(e)
            LOGGER.error('exec_proc catch error:{0}'.format(result))
            if not self.check_is_connected():
                self.start_reconnect()
            return False



    #直接执行存储过程
    def exec_proc_get_one_result(self,sql):
        if self.connection == False :
            #执行之前如果当前连接断开 尝试重连
            if not self.start_reconnect():
                return  False,DB_ERROR_MSG

        try:
            with lock:
                cursor = self.connection.cursor()
                LOGGER.info('exec sql ' +sql)
                cursor.execute(sql)
                result = cursor.fetchone()
                cursor.close()
                return True,result 
        except Exception as e:
            result = str(e)
            LOGGER.error('exec_proc_get_one_result catch error:{0}'.format(result))
            if not self.check_is_connected():
                self.start_reconnect()
                result = DB_ERROR_MSG

            return False,result

                        
    def commit(self):
        self.connection.commit()




if __name__ == '__main__':
    import config
    db = DBUnit(config.DB_USER,config.DB_PWD,config.DB_HOST,config.DB_NAME) 
    # db = DBUnit('root','123','localhost','test') 

    sql = "exec dbo.tools_queryAccbyRoleName '{0}'".format('0442')
    success,result = db.query_proc(sql)
    print(result)
    acc = ''
    if success and len(result) >= 1:
        acc = result[0]


    exit()


    if db.connection == False:
        print ('wrong con')
    else:
        rs = db.check_is_connected()
        print(rs)

        while True:
            time.sleep(1)
            print(db.check_is_connected())
            print(db.exec_proc_get_one_result('select * from auth'))



