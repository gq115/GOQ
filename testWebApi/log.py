# coding=utf-8
import logging
import time
import os

LOG_FORMAT = "time:%(asctime)s   filename:%(filename)s   funcname:%(funcName)s   Levelname:%(levelname)s   message:%(message)s"
DATE_FORMAT = "%Y-%m-%d %H:%M:%S"


log_path = os.getcwd() + "/Log/"
if os.path.exists(log_path) == False:
    os.makedirs(log_path)
curTime = time.strftime("%Y%m%d%H%M", time.localtime())
log_path = log_path + "Log_" + curTime + ".log"


logging.basicConfig(
    level=logging.INFO,
    format=LOG_FORMAT,
    datefmt=DATE_FORMAT,
    filename=log_path,
    filemode='w',
)

LOG = logging.getLogger(__name__)

if __name__ == '__main__':
    LOG.debug("debug message")
    LOG.info("info Message")
    LOG.warning("warning message")
    LOG.critical("critical message")