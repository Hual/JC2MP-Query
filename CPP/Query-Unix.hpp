#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <cstring>
#include <sys/stat.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

class JCMPQuery
{
	public:
		JCMPQuery(std::string, int);
		~JCMPQuery();
		bool update();
		bool isErroneous();
		int getMaxPlayers();
		int getPlayers();
		std::string getHostName();
	private:
		int _players;
		int _max_players;
		std::string _hostname;
		std::string _address;
		int _port;
		bool _error;
};
