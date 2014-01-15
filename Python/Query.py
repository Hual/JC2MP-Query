import socket

class JCMPQuery:

	def __init__(self, address, port = 7777):
		self.address = address
		self.port = port
		self.update()

	def update(self):
		sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
		error = False
		try:
			sock.settimeout(2)
			sdata = b'\xff\xff\xff\xff\x54\x53\x6f\x75\x72\x63\x65\x20\x45\x6e\x67\x69\x6e\x65\x20\x51\x75\x65\x72\x79\0'
			sock.sendto(sdata, (self.address, self.port))
			rdata = sock.recv(256)[6:].split('\0', 3)
			pi = rdata[1][9:].split('/', 2)
			self.hostname = rdata[0]
			self.players = int(pi[0])
			self.max_players = int(pi[1])
		except Exception:
			error = True

		sock.close()
		self.error = error
		return error

	def isErroneous(self):
		return self.error

	def getHostName(self):
		return self.hostname

	def getPlayers(self):
		return self.players

	def getMaxPlayers(self):
		return self.max_players

