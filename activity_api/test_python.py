#coding=utf-8
import requests
import random
import time
from common import get_param_md5



host = 'http://127.0.0.1:8878/'
host = 'http://47.103.54.67:8878/'

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
    odict['acc'] = 'QQ840829192'
    odict['time'] = int(time.time())
    odict['time'] = 1555489094

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


    # charid = __get('charid')
    # itemid = __get('itemid')
    # itemcount = __get('itemcount')
    # title = __get('title')
    # contents = __get('contents')
    # time = __get('time')
    # sign= __get('sign')


def __addItem(api):
    #订单号不能一样
    odict = ODict()

    odict['charid'] = '26E7FE57-C60C-45DC-9EBC-50711D832F59'#南瓜
    odict['itemid'] = 399627930
    odict['itemcount'] = 1

    odict['title'] = '测试'
    odict['contents'] = '真是测试'
    odict['time'] = int(time.time())
    sign = get_param_md5( *[odict[k] for k in odict] )

    odict['sign'] = sign
    #odict['sign'] = 'pip'

    test_api(api,odict)




# __queryAccDayCash()
# api = 'queryTodayRoleOnlineInfo'
# __queryAccApi(api)

# api = 'queryAccRoleListInfo'
# __queryAccApi(api)
__addItem('addItemV2')

# import time


# for j in range(1,10):
#     for i in range(1,100):
#         api = 'queryAccCash'
#         __queryAccApi(api)
#         # time.sleep(0.2)

#     print('one turn' + str(j))
# for i in range(1,1000):
#     __SubAccCash()

# api = 'queryAccRoleEnchantInfo'
# __queryAccApi(api)

