import requests
import random
import time
from common import get_param_md5
MD5_KEY = '123456'

host = 'http://127.0.0.1:8888/'
from collections import OrderedDict as ODict





    #result = requests.get(url)
    #print(result.content)   


i=get_param_md5('1026','123')
print (i)

url_template = host + 'readroleitem?roleid={0}&time={1}&sign={2}'
roleid='1026'
time='123'
url = url_template.format(roleid, time, i)
print('the url is '+ url)