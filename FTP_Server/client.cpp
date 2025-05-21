#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <arpa/inet.h>
#include <filesystem>

#include "Support.h"

namespace fs = std::filesystem;

std::vector<std::string> split(std::string s, char del)
{
    std::vector<std::string> res;
    std::stringstream ss(s);
    std::string word;
    while (!ss.eof())
    {
        getline(ss, word, del);
        res.push_back(word);
    }
    return res;
}

int main()
{
    std::string ip;
    int port;

    while (true)
    {
        std::cout << "Enter command: ";
        std::string input;
        std::getline(std::cin, input);
        auto inputs = split(input, ' ');
        if (inputs.at(0) == "QUIT")
        {
            break;
        }
        else if (inputs.at(0) == "PORT")
        {
            auto value = split(inputs.at(1), ',');
            size_t i;
            for (i = 0; i < 4; i++)
            {
                ip.append(value.at(i));
                if (i != 3)
                {
                    ip.append(".");
                }
            }
            port = std::stoi(value.at(i++));
            port *= 256;
            port += std::stoi(value.at(i));
            break;
        }
        else
        {
            std::cout << "WRONG COMMAND, PLEASE CONNECT SERVER" << std::endl;
        }
    }
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());

    if (connect(clientSocket, reinterpret_cast<sockaddr *>(&serverAddress),
                sizeof(serverAddress)) < 0)
    {
        std::cerr << "Failed to connect to server" << std::endl;
    }

    // login
    while (true)
    {
        std::cout << "Enter command: ";
        std::string input;
        std::getline(std::cin, input);
        auto inputs = split(input, ' ');
        if (inputs.at(0) == "QUIT")
        {
            break;
        }
        else if (inputs.at(0) == "USER")
        {
            send(clientSocket, inputs[1].c_str(), inputs[1].size(), 0);
            uint32_t resp;
            recv(clientSocket, &resp, sizeof(resp), 0);
            if (resp == ERROR)
            {
                std::cout << "WRONG USERNAME" << std::endl;
            }
            else
            {
                break;
            }
        }
        else
        {
            std::cout << "WRONG COMMAND, PLEASE LOGIN SERVER" << std::endl;
        }
    }
    
    // password
        while (true)
    {
        std::cout << "Enter command: ";
        std::string input;
        std::getline(std::cin, input);
        auto inputs = split(input, ' ');
        if (inputs.at(0) == "QUIT")
        {
            break;
        }
        else if (inputs.at(0) == "PASS")
        {
            send(clientSocket, inputs[1].c_str(), inputs[1].size(), 0);
            uint32_t resp;
            recv(clientSocket, &resp, sizeof(resp), 0);
            if (resp == ERROR)
            {
                std::cout << "WRONG PASSWORD" << std::endl;
            }
            else
            {
                break;
            }
        }
        else
        {
            std::cout << "WRONG COMMAND, PLEASE WRITE PASSWORD" << std::endl;
        }
    }

    std::string input;
    while (true)
    {
        std::cout << "Enter command: ";
        std::getline(std::cin, input);

        auto inputs = split(input, ' ');
        input = inputs.at(0);

        Command command;
        char buffer[BUFFER_SIZE] = {};

        if (input == "QUIT")
        {
            break;
        }
        else if (input == "LIST")
        {
            command = Command::LIST;
            send(clientSocket, &command, sizeof(command), 0);
            recv(clientSocket, &buffer, sizeof(buffer) - 1, 0);
            std::cout << buffer << std::endl;
        }
        else if (input == "RETR")
        {
            command = Command::RETR;
            send(clientSocket, &command, sizeof(command), 0);
            send(clientSocket, inputs[1].c_str(), inputs[1].size(), 0);

            uint32_t resp;
            recv(clientSocket, &resp, sizeof(resp), 0);

            if (resp == ERROR)
            {
                recv(clientSocket, &buffer, sizeof(buffer) - 1, 0);
                std::cout << buffer << std::endl;
            }
            else
            {
                uint32_t lengh;
                recv(clientSocket, &lengh, sizeof(lengh), 0);
                std::ofstream myfile;
                fs::path p(inputs[1]);

                myfile.open(p);
                myfile.clear();
                while (lengh > 0)
                {
                    auto bytes = recv(clientSocket, &buffer, std::min(BUFFER_SIZE, lengh), 0);
                    myfile.write(buffer, bytes);
                    lengh -= bytes;
                }

                myfile.close();
            }
        }
        else if (input == "STOR")
        {
            command = Command::STOR;
            fs::path p(inputs[1]);

            if (!fs::exists(p) || !fs::is_regular_file(p))
            {
                std::cout << "File does not exist or is not a regular file" << std::endl;
                continue;
            }

            send(clientSocket, &command, sizeof(command), 0);

            uint32_t name_size = inputs[1].size();
            send(clientSocket, &name_size, sizeof(name_size), 0);
            send(clientSocket, inputs[1].c_str(), name_size, 0);
            

            uint32_t lengh = fs::file_size(p);

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
        else if (input == "CWD")
        {
            command = Command::CWD;
            u_int32_t resp;
            send(clientSocket, &command, sizeof(command), 0);
            send(clientSocket, inputs[1].c_str(), inputs[1].size(), 0);
            recv(clientSocket, &resp, sizeof(resp), 0);

            if (resp == ERROR)
            {
                recv(clientSocket, &buffer, sizeof(buffer) - 1, 0);
                std::cout << buffer << std::endl;
            }
        }
        else if (input == "PWD")
        {
            command = Command::PWD;
            send(clientSocket, &command, sizeof(command), 0);
            recv(clientSocket, &buffer, sizeof(buffer) - 1, 0);
            std::cout << buffer << std::endl;
        }
        else if (input == "MKD")
        {
            command = Command::MKD;
            send(clientSocket, &command, sizeof(command), 0);
            send(clientSocket, inputs[1].c_str(), inputs[1].size(), 0);
            u_int32_t resp;
            recv(clientSocket, &resp, sizeof(resp), 0);
            if (resp == ERROR)
            {
                recv(clientSocket, &buffer, sizeof(buffer) - 1, 0);
                std::cout << buffer << std::endl;
            }
        }
        else if (input == "RMD")
        {
            command = Command::RMD;
            send(clientSocket, &command, sizeof(command), 0);
            send(clientSocket, inputs[1].c_str(), inputs[1].size(), 0);
            u_int32_t resp;
            recv(clientSocket, &resp, sizeof(resp), 0);
            if (resp == ERROR)
            {
                recv(clientSocket, &buffer, sizeof(buffer) - 1, 0);
                std::cout << buffer << std::endl;
            }
        }
        else if (input == "DELE")
        {
            command = Command::DELE;
            send(clientSocket, &command, sizeof(command), 0);
            send(clientSocket, inputs[1].c_str(), inputs[1].size(), 0);
            u_int32_t resp;
            recv(clientSocket, &resp, sizeof(resp), 0);
            if (resp == ERROR)
            {
                recv(clientSocket, &buffer, sizeof(buffer) - 1, 0);
                std::cout << buffer << std::endl;
            }
        }
        else
        {
            std::cout << "WRONG COMMAND" << std::endl;
        }
    }

    close(clientSocket);

    return 0;
}
