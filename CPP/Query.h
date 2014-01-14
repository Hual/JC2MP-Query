/*
       	ANSI C/++ JC2-MP server query function by King_Hual
        

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

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <string>
#if (defined(WIN32) || defined(_WIN32) || defined(_WIN64))
	#define WIN32_LEAN_AND_MEAN
	#include <winsock2.h>
	#include <Ws2tcpip.h>
	#if defined _MSC_VER
		#pragma comment (lib, "Ws2_32.lib")
	#endif
#else
	#include <sys/socket.h>
	#include <netdb.h>
	#include <sys/stat.h>
	#include <netinet/in.h>
	#include <unistd.h>
	#include <errno.h>
	#include <arpa/inet.h>
#endif

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
		void closeSock(int);
};
