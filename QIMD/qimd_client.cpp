#include "qimd_client.h"

qimd_client::qimd_client(int port)
{
    connectPort = port;
    isCreateSockFailed = false;
}

int qimd_client::launch_client()
{
    net::init();
    struct addrinfo* result = NULL, * ptr = NULL, hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int iResult;

    std::cout << "Getting address info..." << std::endl;
    iResult = getaddrinfo(NULL, "61270", &hints, &result);
    if (iResult != 0) {
        std::cout << "GetAddrInfo failed: " << iResult << std::endl;
        WSACleanup();
        return 1;
    }

    SOCKET ConnectSocket = INVALID_SOCKET;

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
        std::cout << "Attempting to build connection socket..." << std::endl;
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET)
        {
            std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
            freeaddrinfo(result);
            WSACleanup();
            return 1;
        }

        std::cout << "Connecting socket..." << std::endl;
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        std::cout << "Unable to connect to server!" << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;       
    }

    std::cout << "Connection socket successfully built!" << std::endl;

    Packet* testpacket = new Packet(Instructions::Instruction::TESTPACKET, "test");
    iResult = send_packet(ConnectSocket, testpacket);

    ParseInput* parser = new ParseInput();

    while (true) {
        std::string userinput;
        std::cout << ">";
        std::getline(std::cin, userinput);
        Instructions::Instruction instr_in = parser->GetInstruction(userinput);
        if (instr_in == Instructions::Instruction::UNRECOGNIZED)
        {
            std::cout << "Instruction was unrecognized! Please enter a valid instruction!" << std::endl;
            continue;
        }
        if (instr_in == Instructions::Instruction::SHUTDOWN)
        {
            break;
        }
        if (instr_in == Instructions::Instruction::EXIT)
        {
            closesocket(ConnectSocket);
            WSACleanup();
            return 0;
        }
        else
        {
            // Split the string into a vector
            std::vector<std::string> container;
            std::istringstream iss(userinput);
            std::copy(std::istream_iterator<std::string>(iss),
                std::istream_iterator<std::string>(),
                std::back_inserter(container));
            std::string datapayload = "";

            for (int i = 1; i < container.size(); i++)
            {
                datapayload.append(container[i]);
                if ((i + 1) < container.size()) datapayload.append(" ");
            }
            if (sizeof(datapayload) > 500)
            {
                std::cout << "Given data was too big to send! Use less data" << std::endl;
            }

            Packet *sendpacket = new Packet(instr_in, datapayload);
            int sendresult = send_packet(ConnectSocket, sendpacket);
            if (sendresult == 1)
            {
                std::cout << "Error" << std::endl;
            }
        }


    }

    std::cout << "Shutting down..." << std::endl;
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        std::cout << "Shutdown failed: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    int recvbuflen = 512;
    char recvbuf[512];

    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            std::cout << "Bytes recieved: " << iResult << std::endl;
        }
        else if (iResult == 0) {
            std::cout << "Connection closed" << std::endl;
        }
        else
        {
            std::cout << "Recv failed" << WSAGetLastError() << std::endl;
        }
    } while (iResult > 0);

    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        std::cout << "Shutdown failed: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    closesocket(ConnectSocket);
    WSACleanup();
    return 0;
}

int qimd_client::send_packet(SOCKET ConnectSocket, Packet *packet)
{
    int iResult;
    int recvbuflen = 512;
    const char* sendbuf = packet->Serialize();
    char recvbuf[512];

    std::cout << "Sending packet..." << std::endl;
    iResult = send(ConnectSocket, sendbuf, 512, 0);
    if (iResult == SOCKET_ERROR) {
        std::cout << "Send failed: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Bytes sent: " << iResult << std::endl;
    return iResult;
}