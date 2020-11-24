import socket
s=socket.socket(socket.AF_INET, socket.SOCK_DGRAM)#sock.SOCK_DGRAM
s.connect(("172.16.38.42", 1337))
s.send(b'GET / HTTP/1.1\r\n\r\n')
