import socket
TCP_IP = '192.168.1.18'
TCP_PORT = 8888
BUFFER_SIZE = 1024  # Normally 1024, but we want fast response

#s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.SO)

#s.connect((TCP_IP, TCP_PORT))
##conn, addr = s.accept()
#print ('Connection address:', addr)
while True:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((TCP_IP, TCP_PORT))
    while 1:
        data = s.recv(BUFFER_SIZE)
        if not data: break
        print("received data:", data.decode('utf-8'))
        #conn.send(data)  # echo
    s.close()


