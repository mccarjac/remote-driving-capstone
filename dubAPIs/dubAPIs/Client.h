#pragma once

struct myPort
{
	struct sockaddr_in other;
	SOCKET socket;
	int otherLen;

};

SOCKET setupClient();
int sendClient(SOCKET * cSock, unsigned char *recvbuf, int bufLength);
int closeClient(SOCKET * cSock);
void * recvClient(SOCKET * cSock, unsigned char * recvbuf, int buflength);

