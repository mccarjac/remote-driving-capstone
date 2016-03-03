#ifndef BBB_H
#define BBB_H

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include "libssh/libssh.h"
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "wheelApi.h"

#define WIN32_LEAN_AND_MEAN
#define DEFAULT_BUFLEN 256
#define DEFAULT_PORT "27015"

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

using namespace std;

class BBBApi {
    public:
        BBBApi();
        ~BBBApi();
        void Send(wheelInputs);
    private:
        void GetIp();
        void Connect();
        string ip;
        SOCKET ConnectSocket;
        int iResult;
};

#endif