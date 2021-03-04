#include <iostream>
#include "ProgramFlags.h"
#include "xsocket.hpp"

void printHelp();

int main(int argc, char *argv[])
{
    std::cout << "Quinton's In-Memory Database" << std::endl;
    
    // Get our command line parameters and store in flags
    char flags = 0;
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-help") == 0)
        {
            printHelp();
            return 0;
        }

        if (strcmp(argv[i], "-s") == 0)
        {
            flags = (flags | SERVER_MODE);
            continue;
        }

        if (strcmp(argv[i], "-c") == 0)
        {
            flags = (flags | CLIENT_MODE);
            continue;
        }
    }

    if (flags == 0)
    {
        printHelp();
        return 0;
    }

    net::socket sock(net::af::inet6, net::sock::dgram, 61270);
    if (!sock.good())
    {
        std::cerr << "error creating socket" << std::endl;
        return -1;
    }
    
    sock.listen()
}

void printHelp()
{
    std::cout << "Help menu for Quinton's In-Memory Database:" << std::endl;
    std::cout << std::endl;
    std::cout << "Parameter    |    Definition" << std::endl;
    std::cout << "       -s    |    Start this instance of the program in server mode" << std::endl;
    std::cout << "       -c    |    Start this instance of the program in client mode" << std::endl;
}

