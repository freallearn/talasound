import socket

TCP_IP = '192.168.1.18'
TCP_PORT = 8888
BUFFER_SIZE = 1024  # Normally 1024, but we want fast response


# s = socket.socket(socket.AF_INET, socket.SOCK_STREAM, socket.SO)

# s.connect((TCP_IP, TCP_PORT))
##conn, addr = s.accept()
# print ('Connection address:', addr)
def is_send2(data):
    print(data)
    return "send" in data


def is_pause() -> bool:
    cond = False
    while True:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((TCP_IP, TCP_PORT))
        data = s.recv(BUFFER_SIZE)
        dec = data.decode()
        if not data:
            s.close()
            break
        if dec.find('pause') != -1:
            print("oui")
            cond = True
            break
    return cond


def receive_commande():
    while True:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((TCP_IP, TCP_PORT))
        data = s.recv(1024)
        if is_send2(data.decode("utf-8")):

            filetodown = open("orders.txt", "wb")
            while True:
                data = s.recv(1024)
                if not data:
                    filetodown.close()
                    break
                filetodown.write(data)
            print("finish")
        # while 1:
        #   data = s.recv(BUFFER_SIZE)
        #  if not data: break
        # print("received data:", data.decode('utf-8'))
        # conn.send(data)  # echo
        s.close()
        break


def launch():
    while True:
        print(is_pause())


launch()
