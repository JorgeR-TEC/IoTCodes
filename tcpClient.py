import socket
s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)#sock.SOCK_DGRAM
s.connect(("3.237.12.191", 11809))
s.send(b'GET / HTTP/1.1\r\n\r\n')
data=s.recv(1024)
print(data)
s.close()