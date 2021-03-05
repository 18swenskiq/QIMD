#include <iostream>
#include "ProgramFlags.h"

#include "xsocket_init.hpp"
#include "xsocket_socket.h"

// Quinton's In-Memory Database is just as the name suggests, an in-memory database.
// This project was made as a senior project to build a large portfolio piece.
// This project uses a very modified version of the xsocket library by wpbirney (https://github.com/wpbirney/xsocket)
// Github repo for this project is at https://github.com/18swenskiq/QIMD

void printHelp();

int main(int argc, char *argv[])
{

    std::cout << "Quinton's In-Memory Database" << std::endl;
    
    int port = 61270; // Default port

    // Get our command line parameters and store in flags
    char user_flags = 0;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-help") == 0)
        {
            printHelp();
            return 0;
        }

        if (strcmp(argv[i], "-s") == 0)
        {
            user_flags = (user_flags | SERVER_MODE);
            continue;
        }

        if (strcmp(argv[i], "-c") == 0)
        {
            user_flags = (user_flags | CLIENT_MODE);
            continue;
        }
    }

    // Print help statement and exit if no arguments
    if (user_flags == 0)
    {
        printHelp();
        return 0;
    }

    // Exit if server and client are defined
    if ((user_flags & SERVER_MODE) && (user_flags & CLIENT_MODE))
    {
        std::cout << "Error: Server and Client mode cannot both be defined!!! Please define only one. Exiting..." << std::endl;
        return 1;
    }

    net::socket sock(net::af::inet6, net::sock::dgram, port);
    if (!sock.good())
    {
        std::cerr << "error creating socket" << std::endl;
        return -1;
    }
    
    sock.listen(5);

    std::cout << "listening on port: " << sock.getlocaladdr().get_port() << std::endl;

    net::socket client;
    net::endpoint remoteAddr;

    while (true) {
        client = sock.accept(&remoteAddr);

        if (client.good()) {
            std::string msg = remoteAddr.get_ip();
            client.send(&msg);
            client.close();
        }
    }
}

void printHelp()
{
    std::cout << "Help menu for Quinton's In-Memory Database:" << std::endl;
    std::cout << std::endl;
    std::cout << "Parameter    |    Definition" << std::endl;
    std::cout << "       -s    |    Start this instance of the program in server mode" << std::endl;
    std::cout << "       -c    |    Start this instance of the program in client mode" << std::endl;
}

