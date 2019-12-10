import hashlib
from log import LOG

MD5_KEY = "123456789"

def get_param_md5(*arg):
    try:
        argList = [str(e) for e in arg]
        srcStr = "".join(argList)
        srcStr = srcStr + MD5_KEY
        md5Obj = hashlib.md5()
        md5Obj.update(bytes(srcStr, encoding="utf-8")) 
        resultStr = md5Obj.hexdigest()
        LOG.info("srcStr: {0}".format(srcStr))
        LOG.info("desStr:{0}".format(resultStr))
        return resultStr
    except Exception as E:
        argList = [str(e) for e in arg]
        srcStr = "".join(argList)
        LOG.info("arg:{0}".format(srcStr))
        return ""


if __name__ == "__main__":
    md5Str = get_param_md5("123", "456", "789")
    print( md5Str )