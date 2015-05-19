#! /usr/bin/python
#! coding: utf-8

import socket

class SocketClosedException(Exception):
	pass

def main():

	sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	sock.bind(('localhost', 6666))
	sock.listen(5)

	while True:
		conn, addr = sock.accept()
                print('connection estb.', addr)
		conn.settimeout(5)

		while True:
			try:
				conn.settimeout(5)
				buf = conn.recv(64)
				if len(buf) == 0:
					raise SocketClosedException()
				print(buf)
				if buf == 'Do you have foundation?':
					conn.send('NOPE.')
				else:
					conn.send('GO AWAY!')
					#conn.close()
			except Exception as e:
				print('connection closed.', e)
				conn.close()
				break

	return 0

if __name__ == '__main__':
	STATUS = main()
