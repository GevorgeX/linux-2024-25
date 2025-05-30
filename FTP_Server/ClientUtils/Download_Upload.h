//
// Created by gevna on 30.05.2025.
//

#ifndef DOWNLOAD_UPLOAD_H
#define DOWNLOAD_UPLOAD_H

namespace fs = std::filesystem;

void retr(Socket& socket, std::string& input) {
    Command command;
    char buffer[4096] = {};
    command = Command::RETR;
    socket.sendData(  &command, sizeof(command) );
    socket.sendData(  input.c_str(), input.size() );

    uint32_t resp;
    socket.recvData(  &resp, sizeof(resp) );

    if (resp == ERROR)
    {
        socket.recvData(  &buffer, sizeof(buffer) - 1 );
        std::cout << buffer << std::endl;
    }
    else
    {
        uint32_t lengh;
        socket.recvData(  &lengh, sizeof(lengh) );
        std::ofstream myfile;
        fs::path p(input);

        myfile.open(p);
        myfile.clear();
        while (lengh > 0)
        {
            auto bytes = socket.recvData(  &buffer, std::min(BUFFER_SIZE, lengh) );
            myfile.write(buffer, bytes);
            lengh -= bytes;
        }

        myfile.close();
    }
}

void stor(Socket& socket,std::string input) {
    Command command;
    char buffer[4096] = {};
    command = Command::STOR;
    fs::path p(input);

    if (!fs::exists(p) || !fs::is_regular_file(p))
    {
        std::cout << "File does not exist or is not a regular file" << std::endl;
        return ;
    }

    socket.sendData(  &command, sizeof(command) );

    uint32_t name_size = input.size();
    socket.sendData(  &name_size, sizeof(name_size) );
    socket.sendData(  input.c_str(), name_size );


    uint32_t lengh = fs::file_size(p);

    socket.sendData(  &lengh, sizeof(lengh) );
    std::ifstream myfile;

    myfile.open(p);

    while (lengh > 0)
    {
        myfile.read(buffer, std::min(BUFFER_SIZE, lengh));
        socket.sendData(  buffer, myfile.gcount() );
        lengh -= myfile.gcount();
    }
    myfile.close();
}
#endif //DOWNLOAD_UPLOAD_H
