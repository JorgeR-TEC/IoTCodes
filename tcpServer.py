import socket
s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(("0.0.0.0", 1337))
s.listen(1)
conn, addr=s.accept()
with conn:
    dataFromClient=conn.recv(1024)
    print(dataFromClient)
    conn.send(b"OK")

