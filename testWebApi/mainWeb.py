import config
import DBConn
import flask
import md5
from log import LOG

webRun = flask.Flask(__name__)
DBManager = DBConn.DBUnit(ip=config.DBINFO["ip"], db=config.DBINFO["DB"], user=config.DBINFO["User"], passw=config.DBINFO["Pass"])

#检查IP是否符合条件
@webRun.before_request
def checkRequesterAlow():
    clientHost = flask.request.headers.get("X-Forwarded-For", flask.request.remote_addr)
    if clientHost not in config.ALOWER_IP:
        LOG.info("{0} not is Alow User".format(clientHost))
        abort(400)
    LOG.info("ip:{0} request:{1}".format(clientHost, str(flask.request)))
#-----------------------------------------------------------------------------

#错误返回信息
def Create_error(errCode, errStr):
    res = {
        'errcode':errCode,
        'errStr':errStr
    }
    return flask.jsonify(res)
#-----------------------------------------------------------------------------

#测试接口
@webRun.route("/testDBProc")
def testDBProc():
    arg1 = flask.request.args.get("arg1", None)
    arg2 = flask.request.args.get("arg2", None)
    arg3 = flask.request.args.get("arg3", None)
    sign = flask.request.args.get("sign", None)

    localSign = md5.get_param_md5(arg1, arg2, arg3)
    if sign != localSign:
        errorStr = "the user sign error. web:{0}, local:{1}".format(sign, localSign)
        LOG.warning(errorStr)
        return Create_error(-3, errorStr)

    procStr = "exec DR2_User.dbo.up_wingmGetUserItem \'743D0CE3-5293-4844-B357-8065667BC1B2\'"
    resultValue, resultStr = DBManager.exec_proc(procStr)
    LOG.info("exec SQL:{0}".format(procStr))
    LOG.info("exec resultValue:{0}, resultStr:{1}".format(resultValue, resultStr))
    res = {
        'success': resultValue,
        'returnStr': resultStr
    }

    return flask.jsonify( res )
#-----------------------------------------------------------------------------

#发送Web道具
@webRun.route("/webSendItem")
def webSendItem():
    Account = flask.request.args.get("Acc", None)
    ChName = flask.request.args.get("Ch", None)
    ItemID = flask.request.args.get("ItemId", None)



if __name__ == "__main__":
    webRun.debug = False
    webRun.run(host=config.LISTEN_IP, port=config.LISTEN_PORT, threaded=True)