#include "socket.h"

mySocket::mySocket() {
    b = new BBBApi();
    
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char buffer[DEFAULT_BUFLEN];
    int bufferlen = DEFAULT_BUFLEN;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        exit(1);
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        exit(1);
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        exit(1);
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(1);
    }

    // No longer need server socket
    closesocket(ListenSocket);
    wheelInputs ins;
    ins.buttons = new bool[23];

    // Receive until the peer shuts down the connection
    do {

        iResult = recv(ClientSocket, buffer, bufferlen, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
            //printf(buffer);
            
            if (buffer[0] == 'q')
                break;
            for (int i = 0;i < 23;i++) {
                if (buffer[i] == '0')
                    ins.buttons[i] = false;
                else
                    ins.buttons[i] = true;
            }
            string temp = buffer;
            stringstream iss(temp);
            string word;
            iss >> word; // Pass buttons input

            iss >> word; // Get gas input
            ins.gas = atoi(word.c_str());

            iss >> word; // Get wheel input
            ins.wheel = atoi(word.c_str());

            iss >> word; // Get brakes
            ins.brakes = atoi(word.c_str());

            iss >> word; // Get dpad
            ins.dpad = atoi(word.c_str());
            b->Send(ins);

            // Echo the buffer back to the sender
            iSendResult = send(ClientSocket, buffer, iResult, 0);
            if (iSendResult == SOCKET_ERROR) {
                printf("send failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                exit(1);
            }
            printf("Bytes sent: %d\n", iSendResult);
        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            exit(1);
        }

    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        exit(1);
    }

    // cleanup
    closesocket(ClientSocket);
    WSACleanup();
    system("pause");
    delete ins.buttons;
}

mySocket::~mySocket() {
    delete b;
}