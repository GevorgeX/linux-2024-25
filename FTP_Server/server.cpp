#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <filesystem>

#include "Support.h"
#include "Thread/ThreadPool/ThreadPool.h"

namespace fs = std::filesystem;

void handle_client(int clientSocket, const std::string &LOGIN, const std::string &PASSWORD)
{

    fs::path current = "/";
    while (true)
    {
        char buffer[BUFFER_SIZE] = {};
        recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);

        std::string login(buffer);
        if (login == LOGIN)
        {
            send(clientSocket, &OK, sizeof(OK), 0);
            break;
            
        }
        else
        {
            send(clientSocket, &ERROR, sizeof(ERROR), 0);
        }
    }
        
    while (true)
    {
        char buffer[BUFFER_SIZE] = {};
        recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);

        std::string pass(buffer);
        if (pass == PASSWORD)
        {
            send(clientSocket, &OK, sizeof(OK), 0);
            break;
            
        }
        else
        {
            send(clientSocket, &ERROR, sizeof(ERROR), 0);
        }
    }

    while (true)
    {
        char buffer[BUFFER_SIZE] = {};
        Command command;
        recv(clientSocket, &command, sizeof(command), 0);
        std::string res;
        if (command == LIST)
        {

            for (const auto &entry : fs::directory_iterator(current))
            {
                res.append(entry.path().filename().string());
                res.append("\n");
            }

            send(clientSocket, res.c_str(), res.length(), 0);
        }
        else if (command == RETR)
        {
            recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
            fs::path p = current / buffer;

            if (!fs::exists(p) || !fs::is_regular_file(p))
            {
                send(clientSocket, &ERROR, sizeof(ERROR), 0);
                res = "retri failed: File doesn't exist or couldn't be opened";
                send(clientSocket, res.c_str(), res.length(), 0);
            }
            else
            {
                send(clientSocket, &OK, sizeof(OK), 0);

                u_int32_t lengh = fs::file_size(p);
                send(clientSocket, &lengh, sizeof(lengh), 0);
                std::ifstream myfile;
                myfile.open(p);
                while (lengh > 0)
                {
                    myfile.read(buffer, std::min(BUFFER_SIZE, lengh));
                    send(clientSocket, buffer, myfile.gcount(), 0);
                    lengh -= myfile.gcount();
                }
                myfile.close();
            }
        }
        else if (command == STOR)
        {
            uint32_t lengh;
            uint32_t name_size;
            recv(clientSocket, &name_size, sizeof(name_size), 0);
            recv(clientSocket, buffer, name_size, 0);

            std::ofstream myfile;
            fs::path p = current / buffer;

            myfile.open(p);
            recv(clientSocket, &lengh, sizeof(lengh), 0);

            myfile.clear();
            while (lengh > 0)
            {
                auto bytes = recv(clientSocket, &buffer, std::min(BUFFER_SIZE, lengh), 0);
                myfile.write(buffer, bytes);
                lengh -= bytes;
            }

            myfile.close();
        }
        else if (command == PWD)
        {
            res = current.string();
            send(clientSocket, res.c_str(), res.length(), 0);
        }
        else if (command == CWD)
        {
            recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
            auto p = current / buffer;

            if (!fs::exists(p) || !fs::is_directory(p))
            {
                send(clientSocket, &ERROR, sizeof(ERROR), 0);
                res = "cdw failed: Path does not exist or is not a directory";
                send(clientSocket, res.c_str(), res.length(), 0);
            }
            else
            {
                send(clientSocket, &OK, sizeof(OK), 0);
                current = p;
            }
        }
        else if (command == RMD)
        {
            recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
            auto p = current / buffer;

            if (!fs::remove_all(p))
            {
                send(clientSocket, &ERROR, sizeof(ERROR), 0);
                res = "rmd failed: Directory not empty or doesn't exist";
                send(clientSocket, res.c_str(), res.length(), 0);
            }
            else
            {
                send(clientSocket, &OK, sizeof(OK), 0);
            }
        }
        else if (command == MKD)
        {
            recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
            auto p = current / buffer;

            if (!fs::create_directory(p))
            {
                send(clientSocket, &ERROR, sizeof(ERROR), 0);
                res = "mkdir failed: Could not create directory (already exists?)";
                send(clientSocket, res.c_str(), res.length(), 0);
            }
            else
            {
                send(clientSocket, &OK, sizeof(OK), 0);
            }
        }
        else if (command == DELE)
        {
            recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
            auto p = current / buffer;
            if (!fs::remove(p))
            {
                send(clientSocket, &ERROR, sizeof(ERROR), 0);
                res = "delete failed: File doesn't exist or couldn't be deleted";
                send(clientSocket, res.c_str(), res.length(), 0);
            }
            else
            {
                send(clientSocket, &OK, sizeof(OK), 0);
            }
        }
    }
}


int main()
{
    const std::string LOGIN = "root";
    const std::string PASSWORD = "1234";




    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(serverSocket, reinterpret_cast<sockaddr *>(&serverAddress),
             sizeof(serverAddress)) < 0)
    {
        std::cerr << "Failed to bind to socket" << std::endl;
        return -1;
    }

    listen(serverSocket, 5);

    const int maxThreads = 10;
    ThreadPool threadPool(maxThreads);
    // Accept a client connection
    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket < 0) {
            std::cerr << "Failed to accept client\n";
            continue;
        }

        std::cout << "Client connected\n";
        threadPool.enqueue(handle_client, clientSocket, LOGIN, PASSWORD);
    }


    close(serverSocket);
    return 0;
}
