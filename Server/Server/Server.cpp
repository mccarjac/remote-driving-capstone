#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/stitching.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_PACKET_SIZE 5000
#define DEFAULT_BUFLEN 2048
#define DEFAULT_PORT "27015"

int __cdecl main(void)
{
	WSADATA wsaData;
	SOCKET s = INVALID_SOCKET;
	SOCKET cSock = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult, iResult;
	char recvBuf[DEFAULT_BUFLEN];
	int recvBufLen = DEFAULT_BUFLEN;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		printf("WSAStartup failed with error - %d\n", iResult);
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("SERVER failed error %d\n", WSAGetLastError());
	}
	printf("Got addrinfo\n");
	s = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (s == INVALID_SOCKET)
	{
		printf("Socket creating failed %d \n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
	}
	printf("Created listen socket\n");

	iResult = bind(s, result->ai_addr, int(result->ai_addrlen));
	if (iResult == SOCKET_ERROR)
	{
		printf("Bind failed with error %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(cSock);
		WSACleanup();
	}
	printf("Bound socket\n");
	freeaddrinfo(result);

	iResult = listen(s, SOMAXCONN);
	if (iResult == SOCKET_ERROR)
	{
		printf("Listen failed with error %d", WSAGetLastError());
		closesocket(s);
		WSACleanup();
	}

	cSock = accept(s, NULL, NULL);
	if (cSock == INVALID_SOCKET)
	{
		printf("SERVER-Accept didn't work %d\n", WSAGetLastError());
		closesocket(s);
		WSACleanup();
	}
	printf("Accept worked\n");
	closesocket(s);

	//Variable declarations...
	cv::Mat  img = cv::Mat::zeros(640, 480, CV_8UC3);
	int  imgSize = (int)(img.total()*img.elemSize());
	printf("We're recieving an image that's |%d|.\n", imgSize);
	char * sockData, *incoming;
	sockData = (char *)malloc(sizeof(char) * (imgSize + 1));
	incoming = (char *)malloc(sizeof(char) * 8);


	while (true)
	{
		//Wait for a HEADS-UP
		/*if ((iResult = recv(cSock, incoming,8, 0)) == SOCKET_ERROR)
		{
			printf("SERVER-REC HEADS-UP Failed with error code %d\n", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		//Compare for heads-up
		if (strncmp(incoming, "HEADS-UP",4) == 0)
		{
			printf("HEADS-UP Received\n");

			//Let the Client know you're ready to recieve
			if ((iResult = send(cSock, "Ready", 5, 0)) == SOCKET_ERROR)
			{
				printf("SERVER-RecvFrom Failed with error code %d\n", WSAGetLastError());
				exit(EXIT_FAILURE);
			}
			else
				printf("SERVER - Notified that I'm ready to recieve.\n");
			*/
		for (int i = 0; i < imgSize; i += DEFAULT_PACKET_SIZE)
		{
			if ((i + DEFAULT_PACKET_SIZE) > imgSize)
			{
				int bytesToTrans = imgSize - i;
				if ((iResult = recv(cSock, sockData + i, bytesToTrans, 0)) == SOCKET_ERROR)
				{
					printf("SERVER- LAST RecvFrom Failed with error code %d\n", WSAGetLastError());
					exit(EXIT_FAILURE);
				}
				else
				{
					printf("Server - (%d/%d)- Bytes received: %d\n", i, imgSize, iResult);
					break;
				}
			}
			else
			{
				if ((iResult = recv(cSock, sockData + i, DEFAULT_PACKET_SIZE, 0)) == SOCKET_ERROR)
				{
					printf("SERVER-RecvFrom Failed with error code %d\n", WSAGetLastError());
					exit(EXIT_FAILURE);
				}
			}
			printf("Server - (%d/%d)- Bytes received: %d\n", i, imgSize, iResult);
		}
		printf("Image data received!\n");
		// Assign pixel value to img
		/*int ptr = 0;
		for (int i = 0; i < img.rows; i++)
		{
			for (int j = 0; j < img.cols; j++)
			{
				assert(img.at<cv::Vec3b>(i, j) != NULL);
				img.at<cv::Vec3b>(i, j) = cv::Vec3b(sockData[ptr], sockData[ptr + 1], sockData[ptr + 2]);
				ptr = ptr + 3;
			}
		}*/
		assert(sockData != NULL);
		img = cv::Mat(cv::Size(640, 480), CV_8UC3, sockData);
		printf("Image constructed.\n");
		cv::imshow("SERVER-side", img);
		if (cvWaitKey(10) >= 0)
		{
			throw(0);
		}
		memset(sockData, '\0', imgSize);
		//memset(incoming, '\0', 8);
		printf("SERVER-Done with iteration\n");
	}

	/*else
	{
		memset(sockData, '\0', imgSize);
		memset(incoming, '\0', 8);
	}*/


	//}

	closesocket(s);
	WSACleanup();
	return 0;
}
