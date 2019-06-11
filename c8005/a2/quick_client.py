#!/bin/python3
from socket import *
import os
import sys
from random import randint
import time
from threading import Thread
import signal


MSG_SIZE=4096

def send_data(ip, port, data, times, rate):
    client_socket = socket(AF_INET, SOCK_STREAM)
    client_socket.connect((ip, port))
    for i in range(times):
        n = client_socket.send(data)
        print ("sent", n)
        n = client_socket.recv(MSG_SIZE)
        print("got", len(n))
    client_socket.close()


ip = ""
port = 0
times = 1
rate = 0.1
threads = 10
if len(sys.argv) != 6:
    sys.exit(1)
ip = sys.argv[1]
port = int(sys.argv[2])
threads = int(sys.argv[3])
rate = float(sys.argv[4])
times = int (sys.argv[5])


pool = []
for i in range(threads):
    #print("generated:", client_socket)
    data = []
    for i in range(MSG_SIZE):
        data.append(randint(97,122))
    data = bytes(data)
    pool.append(Thread(target=send_data, args=(ip, port, data, times, rate)))


print(pool)
for thread in pool:
    thread.start()
    time.sleep(rate)