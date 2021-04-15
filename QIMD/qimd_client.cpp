#include "qimd_client.h"

qimd_client::qimd_client(int port)
{
    connectPort = port;
    sock = nullptr;
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

    int recvbuflen = 512;
    const char* sendbuf = "this is a test";
    char recvbuf[512];

    std::cout << "Sending data..." << std::endl;
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        std::cout << "Send failed: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Bytes sent: " << iResult << std::endl;
    std::cout << "Shutting down..." << std::endl;
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        std::cout << "Shutdown failed: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

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

    //sock = new net::socket(net::af::inet6, net::sock::stream, connectPort);
    //if (!sock->good())
    //{
        //std::cerr << "error creating socket. error: " << sock->geterror() << std::endl;
        //isCreateSockFailed = true;
        //return;
    //}

    //sock->listen(5);
    //std::cout << "Client listening on port: " << sock->getlocaladdr().get_port() << std::endl;

    //sock->connect(sock->getlocaladdr());
    //const std::string teststring = "hello";
    //sock->send(&teststring);

    //while (true) {
        //client = sock->connect(sock->getlocaladdr());

        //if (client.good()) {
            //std::string msg = remoteAddr.get_ip();
            //client.send(&msg);
            //client.close();
        //}
        //else
        //{
            //std::cout << "client not good at " << sock->getlocaladdr().get_ip() << std::endl;
            //std::this_thread::sleep_for(std::chrono::seconds(2));
        //}
    //}
}