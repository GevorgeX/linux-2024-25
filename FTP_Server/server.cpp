#include <fstream>
#include <iostream>
#include <filesystem>

#include "Support.h"
#include "Thread/ThreadPool/ThreadPool.h"
#include "Socket.h"
#include  "ServerUtils/Session.h"
#include "ServerUtils/Direction.h"
#include "ServerUtils/Download_Upload.h"


void handle_client(Socket socket, const std::string &LOGIN, const std::string &PASSWORD)
{

    fs::path current = "/";
    login(socket, LOGIN);
        
    password(socket, PASSWORD);

    while (true)
    {
        Command command;
        socket.recvData( &command, sizeof(command));
        if (command == LIST)
        {
            list(socket, current);
        }
        else if (command == RETR)
        {
            retr(socket, current);
        }
        else if (command == STOR)
        {
            stor(socket, current);
        }
        else if (command == PWD)
        {
            pwd(socket, current);
        }
        else if (command == CWD)
        {
            cwd(socket, current);
        }
        else if (command == RMD)
        {
            rmd(socket, current);
        }
        else if (command == MKD)
        {
            mkd(socket, current);
        }
        else if (command == DELE)
        {
            dele(socket, current);
        }
    }
}


int main()
{
    const std::string LOGIN = "root";
    const std::string PASSWORD = "1234";


    Socket socket;
    socket.bindTo("127.0.0.1", 8080);

    socket.listen();

    const int maxThreads = 10;
    ThreadPool threadPool(maxThreads);
    while (true) {
        try {
            auto client = socket.accept();
            std::cout << "Client connected\n";
            threadPool.enqueue(handle_client, client, LOGIN, PASSWORD);
        }
        catch (const std::exception& e) {
            std::cout << e.what() << "\n";
        }

    }

    return 0;
}
