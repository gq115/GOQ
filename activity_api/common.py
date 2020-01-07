#!/usr/bin/env python
# -*- coding: utf-8 -*-

import hashlib
from config import MD5_KEY


def get_param_md5(*k):
    eles = [str(e) for e in k]
    _str = ''.join(eles)
    _str = _str + MD5_KEY
    h=hashlib.md5()
    h.update(bytes(_str,encoding='utf-8'))
    result = h.hexdigest()
    return result


def get_param_md5_py2(*k):
    from hashlib import md5
    m = md5()
    eles = [str(e) for e in k]
    _str = ''.join(eles)
    _str = _str + MD5_KEY
    m.update(_str)
    result = m.hexdigest()
    return result


if __name__ == '__main__':
    test_md5 = ['1025',123,'ppp']
    _md5 = get_param_md5(test_md5)
    print(_md5)
