#!/bin/python3
import socket

from socket import AF_INET
from socket import SOCK_STREAM
from selectors import select

class connection(object):
    def __init__(self):
        pass

    def create_client(self):
        self.socket = socket.socket(AF_INET, SOCK_STREAM)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
        self.socket.setblocking(0)
        self.socket.bind(("", 0))
        print(self.socket)
        return self.socket

    def create_listener(self, port, address="0.0.0.0", epoll:select.epoll = None):
        self.socket = socket.socket(AF_INET, SOCK_STREAM)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEPORT, 1)
        self.socket.setblocking(0)

        self.socket.bind((address, port))
        self.socket.listen(1)
        if epoll is not None:
            epoll.register(self.socket.fileno(), select.EPOLLIN | select.EPOLLET)
        #print(self.socket)
        """(sock, addr_info) = self.local.accept()
        print(sock, addr_info)
        data = sock.recv(1024)
        while data:
            print(data)
            data = sock.recv(1024)
        print("done")
        """
        return self.socket
