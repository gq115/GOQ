#coding=utf-8
import requests
import random
import time

from common import get_param_md5
import yaml



with open('test.yaml',encoding='utf-8') as f:
    res = yaml.load(f,Loader=yaml.FullLoader)
    host = res['host']
    acc = res['acc']
    test_count = res['test_count']

    print(host)
    print(acc)
    print(test_count)

from collections import OrderedDict as ODict



def test_webpay(orderno, username, gamemoney, time, sig):
    url_template = host + 'webpay?orderno={0}&username={1}&gamemoney={2}&time={3}&sign={4}'

    url = url_template.format(orderno, username, gamemoney, time, sig)
    print('the url is '+ url)

    result = requests.get(url)
    print(result.content)


def test_api(api,orderdict):
    param_url  = '&'.join(['{0}={1}'.format(k,orderdict[k]) for k in  orderdict])
    url = '{host}{api}?{params}'.format(host=host,api=api,params=param_url)
    print('the url is ',url)
    result = requests.get(url)
    print(result.content)





def __test_webpay():
    #订单号不能一样
    orderno = str(random.randint(1,1000000))
    orderno = 10
    print(orderno)
    username = 'gsx'
    gamemoney = random.randint(1,100)
    _time = int(time.time())

    sig = get_param_md5(orderno, username, gamemoney, _time)
    print('the sig is ',sig)

    test_webpay(orderno, username, gamemoney, _time, sig)

def __chagnePasswd():
    #订单号不能一样
    odict = ODict()

    odict['username'] = 'gsx'
    odict['passwd'] = 'abcde'
    odict['time'] = int(time.time())

    sign = get_param_md5( *[odict[k] for k in odict] )
    odict['sign'] = sign

    test_api('changuser',odict)

def __sendItem():
    #订单号不能一样
    odict = ODict()

    odict['chaid'] = 1025
    odict['itemid'] = 1000
    odict['itemcount'] = 2
    odict['mailbt'] = '122'
    odict['mailnr'] = '1434'

    odict['time'] = int(time.time())

    sign = get_param_md5( *[odict[k] for k in odict] )
    print(sign)
    odict['sign'] = sign

    test_api('senditem',odict)


def __queryRoleListByAcc():
    #订单号不能一样
    odict = ODict()
    odict['acc'] = 'gsx4'
    odict['time'] = int(time.time())

    sign = get_param_md5( *[odict[k] for k in odict] )
    odict['sign'] = sign

    test_api('rolelistbyacc',odict)


def __queryAccByRoleName():
    #订单号不能一样
    odict = ODict()
    odict['name'] = '123'
    odict['time'] = int(time.time())

    sign = get_param_md5( *[odict[k] for k in odict] )
    odict['sign'] = sign
    #odict['sign'] = 'pip'

    test_api('accbyrolename',odict)


def __queryAccDayCash():
    #订单号不能一样
    odict = ODict()
    odict['acc'] = acc
    odict['time'] = int(time.time())

    sign = get_param_md5( *[odict[k] for k in odict] )
    odict['sign'] = sign
    #odict['sign'] = 'pip'

    test_api('queryTodayCashConsume',odict)



def __SubAccCash():
    #订单号不能一样
    odict = ODict()
    odict['acc'] = 'gsx'
    odict['cash'] = 1
    odict['time'] = int(time.time())

    sign = get_param_md5( *[odict[k] for k in odict] )
    odict['sign'] = sign
    #odict['sign'] = 'pip'

    test_api('subAccCash',odict)


def __queryAccApi(api):
    #订单号不能一样
    odict = ODict()
    odict['acc'] = 'gsx'
    odict['time'] = int(time.time())

    sign = get_param_md5( *[odict[k] for k in odict] )
    odict['sign'] = sign
    #odict['sign'] = 'pip'

    test_api(api,odict)

for i in range(test_count):
    __queryAccDayCash()

# __queryAccDayCash()
# api = 'queryTodayRoleOnlineInfo'
# __queryAccApi(api)

# api = 'queryAccRoleListInfo'
# __queryAccApi(api)


# api = 'queryAccCash'
# __queryAccApi(api)

# __SubAccCash()

# api = 'queryAccRoleEnchantInfo'
# __queryAccApi(api)

