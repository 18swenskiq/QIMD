#include "qimd_server.h"

#define SOCKETERROREXIT(s1, s2, r, ls) do {\
        std::cout << s1 << s2 << std::endl;\
        freeaddrinfo(r);\
        closesocket(ls);\
        WSACleanup();\
        return 1;\
    } while(0)

qimd_server::qimd_server(int port)
{
    connectPort = port;
    isCreateSockFailed = false;
    numtables = 0;
}

int qimd_server::launch_server()
{
    net::init();
    struct addrinfo* result = NULL, *ptr = NULL, hints;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    int iResult;

    std::cout << "Getting addr info..." << std::endl;
    iResult = getaddrinfo(NULL, "61270", &hints, &result);
    if (iResult != 0) {
        std::cout << "getaddrinfo failed:" << iResult << std::endl;
    }

    SOCKET ListenSocket = INVALID_SOCKET;

    std::cout << "Building listen socket..." << std::endl;
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        SOCKETERROREXIT("Error at socket() : ", WSAGetLastError(), result, ListenSocket);
        //std::cout << "Error at socket() : " << WSAGetLastError() << std::endl;
        //freeaddrinfo(result);
        //WSACleanup();
        //return 1;
    }

    std::cout << "Binding listen socket..." << std::endl;
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        SOCKETERROREXIT("Bind failed with error: ", WSAGetLastError(), result, ListenSocket);
        //std::cout << "Bind failed with error: " << WSAGetLastError() << std::endl;
        //freeaddrinfo(result);
        //closesocket(ListenSocket);
        //WSACleanup();
        //return 1;
    }

    std::cout << "Testing listen..." << std::endl;
    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
        SOCKETERROREXIT("Listen failed with error: ", WSAGetLastError(), result, ListenSocket);
        //std::cout << "Listen failed with error " << WSAGetLastError() << std::endl;
        //closesocket(ListenSocket);
        //WSACleanup();
        //return 1;
    }

    SOCKET ClientSocket;
    ClientSocket = INVALID_SOCKET;

    std::cout << "Preparing socket for client..." << std::endl;
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        SOCKETERROREXIT("Accept failed: ", WSAGetLastError(), result, ListenSocket);
        //std::cout << "Accept failed: " << WSAGetLastError() << std::endl;
        //closesocket(ListenSocket);
        //WSACleanup();
        //return 1;
    }

    char recvbuf[512];
    int iSendResult;
    int recvbuflen = 512;

    std::cout << "Server listening on port " << connectPort << std::endl;
    do {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            Packet* recvpacket = new Packet(recvbuf);
            int parseresult = parse_packet(recvpacket);
            if (parseresult == 1)
            {
                std::cout << "Malformed packet" << std::endl;
            }
            std::cout << "Bytes received: " << iResult << std::endl;
            iSendResult = send(ClientSocket, recvbuf, iResult, 0);
            if (iSendResult == SOCKET_ERROR) {
                SOCKETERROREXIT("Send failed: ", WSAGetLastError(), result, ListenSocket);
                //std::cout << "Send failed: " << WSAGetLastError() << std::endl;
                //closesocket(ClientSocket);
                //WSACleanup();
                //return 1;
            }
            std::cout << "Bytes send: " << iSendResult << std::endl;
        }
        else if (iResult == 0) {
            std::cout << "Connection closing..." << std::endl;
        }
        else {
            SOCKETERROREXIT("Recv failed: ", WSAGetLastError(), result, ListenSocket);
            //std::cout << "Recv failed: " << WSAGetLastError();
            //closesocket(ClientSocket);
            //WSACleanup();
            //return 1;
        }
    } while (iResult > 0);

    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        SOCKETERROREXIT("Shutdown failed: ", WSAGetLastError(), result, ListenSocket);
        //std::cout << "Shutdown failed: " << WSAGetLastError() << std::endl;
        //closesocket(ClientSocket);
        //WSACleanup();
        //return 1;
    }

    closesocket(ClientSocket);
    WSACleanup();
    return 0;
}

int qimd_server::parse_packet(Packet* packet)
{
    // Make sure header is correct
    if (packet->Begin != Instructions::Instruction::STARTPACKET)
    {
        return 1;
    }
    // Make sure footer is correct
    if (packet->End != Instructions::Instruction::ENDPACKET)
    {
        return 1;
    }

    // Check instruction and perform operations
    if (packet->InputInstruction == Instructions::Instruction::NEWTABLE)
    {
        tables.push_back(new QIMDTable(packet->Data));
        numtables++;
    }

    return 0;
}
