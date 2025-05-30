#ifndef SESSION_H
#define SESSION_H

#include <iostream>

void login(Socket& socket, const std::string &LOGIN) {
    while (true)
    {
        char buffer[BUFFER_SIZE] = {};
        socket.recvData( buffer, BUFFER_SIZE - 1);

        std::string login(buffer);
        if (login == LOGIN)
        {
            socket.sendData( &OK, sizeof(OK));
            break;

        }
        else
        {
            socket.sendData( &ERROR, sizeof(ERROR));
        }
    }
}

void password(Socket& socket, const std::string &PASSWORD) {
    while (true)
    {
        char buffer[BUFFER_SIZE] = {};
        socket.recvData( buffer, BUFFER_SIZE - 1);

        std::string pass(buffer);
        if (pass == PASSWORD)
        {
            socket.sendData( &OK, sizeof(OK));
            break;

        }
        else
        {
            socket.sendData( &ERROR, sizeof(ERROR));
        }
    }
}

#endif //SESSION_H
