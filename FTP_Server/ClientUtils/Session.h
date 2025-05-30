#ifndef SESSION_H
#define SESSION_H

#include <iostream>

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

void parse_address(std::string& ip, int &port) {
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
}

void login(Socket& socket) {
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
            socket.sendData( inputs[1].c_str(), inputs[1].size());
            uint32_t resp;
            socket.recvData( &resp, sizeof(resp));
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
}

void password(Socket& socket) {
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
            socket.sendData( inputs[1].c_str(), inputs[1].size());
            uint32_t resp;
            socket.recvData(&resp, sizeof(resp));
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
}

#endif //SESSION_H
