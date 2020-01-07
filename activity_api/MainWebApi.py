# -*- coding: utf-8 -*-
import datetime



from flask import Flask,request,redirect,url_for,abort 
from flask import jsonify as t_jsonify

import config
from config import ERRCODE,ERRDESC
from common import get_param_md5

from mylog import LOGGER

import sys


app = Flask(__name__)


def __get(name,default_val=''):
    buf =request.args.get(name,default_val)
    # print (buf)
    return buf


#统一 的返回经过路径
def jsonify(dic):
    # print('jsonify',dic)

    if config.LOG_REQUEST_RESPONSE:
        def write_log(content):
            file_path = 'req_res.log'
            file = open(file_path,'a+')
            timeStr = "\n"+str(datetime.datetime.now())+ '  '
            file.write(timeStr+str(content)+'\n')
            file.close()

        if config.LOG_REQUEST_RESPONSE:
            write_log("*"*20)
            write_log("request is " + str(request))
            write_log('the return is '+str(dic))
            write_log("*"*20)

    return t_jsonify(dic)


def create_error(errcode,desc=""):
    if desc == "" :
        desc = ERRDESC.get(errcode,"")

    res = {
        'errcode':errcode,
        'desc':desc
    }
    return jsonify(res)




@app.before_request
def checkRequestValid():
    '''
        监测ip地址是否是合法ip
    '''
    if config.CHECK_IP:
        ip=request.headers.get('X-Forwarded-For',request.remote_addr)
        if ip not in config.ALLOW_LIST:
            LOGGER.error('checkRequestValid failured' + ip)
            abort(400)

        LOGGER.error('ip{0} : request: {1}'.format(ip,str(request)))




#检测 get 请求的参数
def __check_get_param(param_name_list):
    for name in param_name_list:
        if request.args.get(name,None) == None:
            LOGGER.error(' request {0} not exist'.format(name))
            return False

    return True




#格式化时间
def _formatDate(val):
    if type(val) == datetime.datetime:
        return val.strftime("%Y-%m-%d") 
    return val



# 查询账号当日花费
@app.route('/queryTodayCashConsume')
def queryTodayCashConsume():
    acc = __get('acc')
    time = __get('time')
    sign= __get('sign')


    return jsonify({
        'success': 1,
        'use_cash':"returndata",
    })



import time
if __name__ == '__main__':
    if config.DEBUG_MODE:
        app.debug = False

    app.run(host=config.LISTEN_IP,threaded=True,port=config.LISTEN_PORT)
    