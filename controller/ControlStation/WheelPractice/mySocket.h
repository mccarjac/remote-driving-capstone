#ifndef MYSOCKET_H
#define MYSOCKET_H

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <winsock2.h>
#define _WINSOCKAPI_
#include <windows.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "wheelApi.h"

#define WIN32_LEAN_AND_MEAN
#define DEFAULT_BUFLEN 256
#define DEFAULT_PORT "27016"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

using namespace std;

class mySocket {
public:
    mySocket();
    ~mySocket();
    void Send(wheelInputs);
private:
    void GetIp();
    void Connect();
    string ip;
    SOCKET ConnectSocket;
    int iResult;
};

#endif