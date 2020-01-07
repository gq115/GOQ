# coding=utf-8
from tornado.wsgi import WSGIContainer
from tornado.httpserver import HTTPServer
from tornado.ioloop import IOLoop
import MainWebApi 
import config

if __name__ == '__main__':
    http_server = HTTPServer(WSGIContainer(MainWebApi.app))
    http_server.listen(config.LISTEN_PORT)
    IOLoop.instance().start()
