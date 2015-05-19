#! /usr/bin/python
# coding: utf-8

import socket
import time
import random

def main():

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(('localhost', 6666))

    while True:
        try:
            #sock.send('Do you have foundation?')
            sock.send('./test_exec.py')
            print(sock.recv(1024))
        except KeyboardInterrupt:
            break

    sock.close()

    return 0

if __name__ == '__main__':
    STATUS = main()
