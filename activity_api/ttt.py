#coding:utf-8

from collections import OrderedDict as ODict


def get_param_md5(*k):
    from hashlib import md5
    m = md5()
    eles = [str(e) for e in k]
    _str = ''.join(eles)
    _str = _str + MD5_KEY
    m.update(_str)
    result = m.hexdigest()
    return result

kk = {
    'age':10,
    'buf':20,
    'ape':20,
    'ttt':20,
    'zwer':20,
    'asd':'西瓜皮'
}
keys = sorted(kk.keys())
odict = ODict()
for k in keys:
    odict[k] = kk[k]

from urllib import urlencode, quote
print(urlencode(odict))




