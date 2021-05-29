import socket

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(("0.0.0.0", 5000))

server.listen(5)

try:
	while True:
		client, addr = server.accept()
		print("Connected by", addr)
		try:
			while True:
				if(client.recv(2048).decode('ascii') == "img"):
					buffer = b""
					while(len(buffer) < 307200):
						buffer += client.recv(2048)
						print(len(buffer))
		except KeyboardInterrupt:
			client.close()
except KeyboardInterrupt:
	server.close()