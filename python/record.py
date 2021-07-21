import socket
import numpy as np
from scipy.io.wavfile import write

HOST = "0.0.0.0"
PORT = 5000

PREFIX = "dataset/front/"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
s.bind((HOST, PORT))
s.listen(5)

cnt = 11

while True:
    conn, addr = s.accept()
    print('connected by ' + str(addr))

    audio = np.zeros(shape=(0, 2), dtype=np.int16)

    while True:
        data = conn.recv(1600)
        if len(data) == 0: # connection closed
            conn.close()
            print('client closed connection.')
            write(f"{PREFIX}{cnt}.wav", 16000, audio)
            np.save(f"{PREFIX}{cnt}", audio)
            cnt += 1
            break
        # print('recv: ' + indata.decode())

        # outdata = 'echo ' + indata.decode()
        # conn.send(outdata.encode())
        audio = np.append(audio,np.frombuffer(data, dtype=np.int16).reshape(-1, 2), axis=0)
        print(len(audio))
