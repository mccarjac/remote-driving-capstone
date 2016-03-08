#ifndef SOCKET_H
#define SOCKET_H

#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <istream>
#include <sstream>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "wheel.h"

class mySocket {
	public:
		mySocket();
	private:
		int sockfd, newsockfd, portno, n;
		socklen_t clilen;
		char buffer[256];
		struct sockaddr_in serv_addr, cli_addr;
		void error(const char*);
};

#endif
