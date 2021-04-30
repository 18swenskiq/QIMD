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
    curtable = nullptr;
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
    }

    std::cout << "Binding listen socket..." << std::endl;
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        SOCKETERROREXIT("Bind failed with error: ", WSAGetLastError(), result, ListenSocket);
    }

    std::cout << "Testing listen..." << std::endl;
    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
        SOCKETERROREXIT("Listen failed with error: ", WSAGetLastError(), result, ListenSocket);
    }

    SOCKET ClientSocket;
    ClientSocket = INVALID_SOCKET;

    std::cout << "Preparing socket for client..." << std::endl;
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        SOCKETERROREXIT("Accept failed: ", WSAGetLastError(), result, ListenSocket);
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
            delete(recvpacket);
            Packet* sendpacket;
            if (parseresult == 1)
            {
                sendpacket = new Packet(Instructions::Instruction::UNRECOGNIZED, "Input was invalid or malformed");
            }
            else
            {
                sendpacket = new Packet(Instructions::Instruction::SUCCESS, "Good");
            }

            iSendResult = send(ClientSocket, sendpacket->Serialize(), iResult, 0);
            if (iSendResult == SOCKET_ERROR) {
                SOCKETERROREXIT("Send failed: ", WSAGetLastError(), result, ListenSocket);
            }
        }
        else if (iResult == 0) {
            std::cout << "Connection closing..." << std::endl;
        }
        else {
            SOCKETERROREXIT("Recv failed: ", WSAGetLastError(), result, ListenSocket);
        }
    } while (iResult > 0);

    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        SOCKETERROREXIT("Shutdown failed: ", WSAGetLastError(), result, ListenSocket);
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
    else if (packet->InputInstruction == Instructions::Instruction::SWITCHTABLE)
    {
        int switchresult = switch_table(packet->Data);
        if (switchresult == 1) return 1;
    }

    return 0;
}

int qimd_server::switch_table(std::string datapayload)
{
    std::string cmpstring = Utils::SplitStringToVector(datapayload)[0];
    for (int i = 0; i < numtables; i++)
    {
        if (cmpstring == tables[i]->Name)
        {
            curtable = tables[i];
            return 0;
        }
    }
    return 1;
}
