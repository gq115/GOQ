#!/usr/bin/env python
# -*- coding: utf-8 -*-


from logging import Formatter, FileHandler
import os
import time
import logging
from logging.handlers import TimedRotatingFileHandler

LOGGER = logging.getLogger('whaterver')


LOGGER.setLevel(logging.INFO)
rq = time.strftime('%Y%m%d', time.localtime(time.time()))
log_path = os.getcwd() + '/Logs/'
isExists=os.path.exists(log_path)
if not isExists:
    os.makedirs(log_path)

logname = log_path +'log_'+ rq + '.log'
file_handler = TimedRotatingFileHandler(filename=logname, when="D", interval=1, backupCount=15)

file_handler.setFormatter(Formatter( '%(asctime)s - %(levelname)s - %(filename)s - %(funcName)s - %(lineno)s - %(message)s' ))
LOGGER.addHandler(file_handler)

if __name__ == '__main__':
    LOGGER.info("西瓜")
    LOGGER.error("苹果")
