#include <iostream>
#include "ProgramFlags.h"

#include "qimd_client.h"
#include "qimd_server.h"

// Quinton's In-Memory Database is just as the name suggests, an in-memory database.
// This project was made as a senior project to build a large portfolio piece.
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

    if (user_flags & SERVER_MODE)
    {
        qimd_server* qimds = new qimd_server(port);
        if (qimds->isCreateSockFailed)
        {
            std::cout << "Error: Server socket creation failed." << std::endl;
            return 1;
        }
        qimds->launch_server();
    }

    if (user_flags & CLIENT_MODE)
    {
        qimd_client* qimdc = new qimd_client(port);
        if (qimdc->isCreateSockFailed)
        {
            std::cout << "Error: Server socket creation failed." << std::endl;
            return 1;
        }
        qimdc->launch_client();
    }

    return 0;
}

void printHelp()
{
    std::cout << "Help menu for Quinton's In-Memory Database:" << std::endl;
    std::cout << std::endl;
    std::cout << "Parameter    |    Definition" << std::endl;
    std::cout << "       -s    |    Start this instance of the program in server mode" << std::endl;
    std::cout << "       -c    |    Start this instance of the program in client mode" << std::endl;
}

