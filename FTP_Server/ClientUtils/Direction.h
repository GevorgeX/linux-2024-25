#ifndef DIRECTION_H
#define DIRECTION_H

namespace fs = std::filesystem;

void list(Socket& socket) {
    Command command = Command::LIST;
     char buffer[4096] = {};
    socket.sendData(  &command, sizeof(command) );
    socket.recvData(  &buffer, sizeof(buffer) - 1 );
    std::cout << buffer << std::endl;
}


void cwd(Socket& socket, std::string input) {
    Command command;
     char buffer[4096] = {};
    command = Command::CWD;
    u_int32_t resp;
    socket.sendData(  &command, sizeof(command) );
    socket.sendData(  input.c_str(), input.size() );
    socket.recvData(  &resp, sizeof(resp) );

    if (resp == ERROR)
    {
        socket.recvData(  &buffer, sizeof(buffer) - 1 );
        std::cout << buffer << std::endl;
    }
}

void pwd(Socket& socket) {
    Command command;
     char buffer[4096] = {};
    command = Command::PWD;
    socket.sendData(  &command, sizeof(command) );
    socket.recvData(  &buffer, sizeof(buffer) - 1 );
    std::cout << buffer << std::endl;
}

void mkd(Socket& socket, std::string input) {
    Command command;
     char buffer[4096] = {};
    command = Command::MKD;
    socket.sendData(  &command, sizeof(command) );
    socket.sendData(  input.c_str(), input.size() );
    u_int32_t resp;
    socket.recvData(  &resp, sizeof(resp) );
    if (resp == ERROR)
    {
        socket.recvData(  &buffer, sizeof(buffer) - 1 );
        std::cout << buffer << std::endl;
    }
}

void rmd(Socket& socket, std::string input) {
    Command command;
     char buffer[4096] = {};
    command = Command::RMD;
    socket.sendData(  &command, sizeof(command) );
    socket.sendData(  input.c_str(), input.size() );
    u_int32_t resp;
    socket.recvData(  &resp, sizeof(resp) );
    if (resp == ERROR)
    {
        socket.recvData(  &buffer, sizeof(buffer) - 1 );
        std::cout << buffer << std::endl;
    }
}

void dele(Socket& socket, std::string input) {
    Command command;
     char buffer[4096] = {};
    command = Command::DELE;
    socket.sendData(  &command, sizeof(command) );
    socket.sendData(  input.c_str(), input.size() );
    u_int32_t resp;
    socket.recvData(  &resp, sizeof(resp) );
    if (resp == ERROR)
    {
        socket.recvData(  &buffer, sizeof(buffer) - 1 );
        std::cout << buffer << std::endl;
    }
}
#endif //DIRECTION_H
