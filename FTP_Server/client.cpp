#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>

#include "Support.h"
#include "Socket.h"
#include "ClientUtils/Session.h"
#include "ClientUtils/Download_Upload.h"
#include "ClientUtils/Direction.h"


int main()
{
    std::string ip;
    int port;

    parse_address(ip, port);
    Socket socket;

    socket.connectTo(ip, port);

    // login
    login(socket);
    
    // password
    password(socket);

    std::string input;
    while (true)
    {
        std::cout << "Enter command: ";
        std::getline(std::cin, input);

        auto inputs = split(input, ' ');
        input = inputs.at(0);

        if (input == "QUIT")
        {
            break;
        }
        else if (input == "LIST")
        {
            list(socket);
        }
        else if (input == "RETR")
        {
            retr(socket, inputs[1]);
        }
        else if (input == "STOR")
        {
            stor(socket, inputs[1]);
        }
        else if (input == "CWD")
        {
            cwd(socket, inputs[1]);
        }
        else if (input == "PWD")
        {
            pwd(socket);
        }
        else if (input == "MKD")
        {
            mkd(socket, inputs[1]);
        }
        else if (input == "RMD")
        {
            rmd(socket, inputs[1]);
        }
        else if (input == "DELE")
        {
            dele(socket, inputs[1]);
        }
        else
        {
            std::cout << "WRONG COMMAND" << std::endl;
        }
    }

    return 0;
}
