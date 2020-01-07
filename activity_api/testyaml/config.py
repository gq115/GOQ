# -*- coding: utf-8 -*-

import os
import yaml

#ip白名单
ALLOW_LIST = None
#记录所有请求和返回

MD5_KEY = 'b83786aaac8d4b80'


with open('config.yaml',encoding='utf-8') as f:
    res = yaml.load(f,Loader=yaml.FullLoader)
    ALLOW_LIST = res['ip_list']

    DB_USER = str(res['db_info']['user']).strip()
    DB_PWD = str(res['db_info']['passwd']).strip()
    DB_HOST = str(res['db_info']['host']).strip()
    DB_NAME = res['db_info']['db_name']
    DEBUG_MODE = res.get('DEBUG_MODE',False)
    CHECK_IP = res.get('CHECK_IP',True)
    LOG_REQUEST_RESPONSE = res.get('LOG_REQUEST_RESPONSE',False)

    LOG_DB_USER = str(res['log_db_info']['user']).strip()
    LOG_DB_PWD = str(res['log_db_info']['passwd']).strip()
    LOG_DB_HOST = str(res['log_db_info']['host']).strip()
    LOG_DB_NAME = res['log_db_info']['db_name']

    LISTEN_IP = res['server_listen']['ip']
    LISTEN_PORT = res['server_listen']['port']


class ERRCODE:
    SUCCESS = 0
    PARAM_ERROR = 1  #参数错误
    MD5_ERROR = 2
    SUB_VALUE_NOT_POSIVE = 3 # 扣除只能是正数


ERRDESC = {
    ERRCODE.PARAM_ERROR:"参数错误",
    ERRCODE.MD5_ERROR:"md5 error",
    ERRCODE.SUB_VALUE_NOT_POSIVE:"扣除只能是正数",
}



if __name__ == '__main__':
    print (LISTEN_IP,'====',LISTEN_PORT)
    print(ALLOW_LIST)
    print(DB_NAME)
    print(LISTEN_IP)
    print(LISTEN_PORT)
    print(DEBUG_MODE)





