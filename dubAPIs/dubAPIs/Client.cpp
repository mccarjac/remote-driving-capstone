
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <string>
#include <time.h>
#include "Client.h"
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define DEFAULT_SERVER "127.0.0.1"
#define DEFAULT_BUFLEN 2048
#define DEFAULT_PORT "27015"


SOCKET setupClient()
{
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;
	SOCKET s = INVALID_SOCKET;
	char buf[DEFAULT_BUFLEN];
	WSADATA wsa;

	printf("CLIENT: Initializing WINsock...\n");
	if (WSAStartup(MAKEWORD(2,2), &wsa) != 0)
	{
		printf("CLIENT: Failure - %d\n", WSAGetLastError());
		
		exit(EXIT_FAILURE);
	}
	else
		printf("Initialized. \n");
	
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int iResult;
		
	iResult = getaddrinfo(DEFAULT_SERVER, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("CLIENT - getaddrinfo failed\n");
		WSACleanup();
	}
	//CLIENT SOCKET CREATION
	if ((s = socket(result->ai_family, result->ai_socktype,result->ai_protocol)) == INVALID_SOCKET)
	{
		printf("CLIENT: Socket creation failure. - %d\n", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	else
		printf("Socket created succesfully\n");
	//Connect to SERVER
	iResult = connect(s, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		printf("CLIENT_Not connecting correctly.\n");
		closesocket(s);
		s = INVALID_SOCKET;
	}
	else
		printf("CLient connected correctly;\n");

	freeaddrinfo(result);

	/*
	if ((iResult = send(s,"Hello?", 7, 0)) == SOCKET_ERROR)
	{
		printf("CLIENT Setup: SendTo error - %d\n", WSAGetLastError());

	}
	printf("Bytes sent: %d\n", iResult);
	*/
	return s;
}

void * recvClient(SOCKET * cSock, unsigned char * recvbuf, int buflength)
{
	memset(recvbuf, '\0', buflength);

	if (recv(*cSock, (char *)recvbuf, buflength, 0) == SOCKET_ERROR)
	{
		printf("CLIENT : REcvfrom failed - %d\n", WSAGetLastError());
		
		exit(EXIT_FAILURE);
	}
	//return (char *)recvbuf;
} 


int sendClient(SOCKET * cSock, unsigned char *recvbuf, int bufLength)
{
	if (send(*cSock, (const char *)recvbuf, bufLength, 0) == SOCKET_ERROR)
	{
		printf("CLIENT: SendTo error - %d\n", WSAGetLastError());
		
		return 1;
	}
	/*
	int iResult,total;
	total = 0;
	iResult = send(*ConnectSocket,(const char *) recvbuf, (int)strlen((const char *)recvbuf), 0);
	//SO THIS IS WHERE IT'S FAILING...
	if (iResult < 0) {
		printf("send failed with error: %d\n", WSAGetLastError());
		printf("Closed connect socket in sendClient..\n");
		closesocket(*ConnectSocket);
		WSACleanup();
		return 1;
	}
	else if (iResult == 0)
	{
		printf("Nothing...\n");
	}
	else
		printf("Bytes sent : %d", iResult);
	
	//Commented here
	do {
		while (total < bufLength)
		{
			printf("Attempting to send...\n");
			total += send(ConnectSocket, (const char *)recvbuf, bufLength-total, 0);
			if (total > 0)
				printf("Bytes sent: %d\n", total);
			else if (total == 0)
				printf("Connection closed\n");
		}
	} while (total < bufLength);*/

	return 0;
}


int closeClient(SOCKET * cSock)
{
	printf("CLOSING CLIENT...\n");

	closesocket(*cSock);
	WSACleanup();
	return 0;
}