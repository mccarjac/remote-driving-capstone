#ifndef MYSOCKET_H
#define MYSOCKET_H

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <istream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "bbb.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27016"

class mySocket {
    public:
        mySocket();
        ~mySocket();
    private:
        BBBApi* b;
};

#endif