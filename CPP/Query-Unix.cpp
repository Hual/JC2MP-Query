/*
	C++ Unix GNU JC2-MP server query function by King_Hual
	

	Arguments:

	string address - The IP address
	int port [optional] - The port

	Methods:

	update() - Updates the information
	isErroneous() - Returns true if any errors occured, false otherwise
	getHostName() - Returns the hostname of a server
	getPlayers() - Returns the amount of players currently connected to the server
	getMaxPlayers() - Returns the maximum amount of players that can be connected to the server

*/

#include "Query-CPP-Unix.hpp"

JCMPQuery::JCMPQuery(std::string address, int port = 7777)
{
	this->_address = address;
	this->_port = port;
	this->update();
}

JCMPQuery::~JCMPQuery()
{
	this->_players = 0;
	this->_max_players = 0;
	this->_hostname = "";
	this->_address = "";
	this->_port = 0;
	this->_error = 0;
}

bool JCMPQuery::isErroneous()
{
	return this->_error;
}

int JCMPQuery::getMaxPlayers()
{
	return this->_max_players;
}

int JCMPQuery::getPlayers()
{
	return this->_players;
}

std::string JCMPQuery::getHostName()
{
	return this->_hostname;
}

bool JCMPQuery::update()
{
	struct addrinfo hints, *servinfo, *p;
	char rdata[256], tmpPort[11];
	int sock, rv;
	memset(rdata, 0, sizeof(rdata));
	memset(&hints, 0, sizeof(hints));
	sprintf(tmpPort, "%d", this->_port);

	hostent *record = gethostbyname(this->_address.c_str());
	if(record == NULL) return (this->_error = true);
	in_addr *address = (in_addr *)record->h_addr;
	if ((rv = getaddrinfo(inet_ntoa(*address), tmpPort, &hints, &servinfo)) != 0)
		return (this->_error = true);

	struct timeval timeout;
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;
	for(p=servinfo; p!=NULL; p=p->ai_next)
	{
		if ((sock = socket(p->ai_family, SOCK_DGRAM,0)) == -1)
			continue;
		setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
		if (connect(sock, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sock);
			continue;
		}
		break;
	}
	if (p == NULL)
		return (this->_error = true);

	freeaddrinfo(servinfo);
	unsigned char sdata[] =
	{
		0xff,0xff,0xff,0xff,0x54,
		0x53,0x6f,0x75,0x72,0x63,
		0x65,0x20,0x45,0x6e,0x67,
		0x69,0x6e,0x65,0x20,0x51,
		0x75,0x65,0x72,0x79,0x00
	};
	if(send(sock, sdata, sizeof(sdata), 0) == -1)
		return (this->_error = true);
	int recv_size = recv(sock, rdata, sizeof(rdata), 0);
	close(sock);
	if(recv_size < 6)
		return (this->_error = true);

	char *pi = &rdata[strcspn(rdata, "\0")+1];
	this->_hostname = (std::string)&rdata[6];
	this->_players = atoi(strtok(&pi[9], "/"));
	this->_max_players = atoi(strtok(NULL, "/"));
	return (this->_error = false);
}
