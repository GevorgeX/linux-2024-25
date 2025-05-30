//
// Created by gevna on 30.05.2025.
//

#ifndef DOWNLOAD_UPLOAD_H
#define DOWNLOAD_UPLOAD_H

namespace fs = std::filesystem;

void retr(Socket& socket, fs::path& current) {
    char buffer[4096] = {};
    std::string res;
    socket.recvData( buffer, BUFFER_SIZE - 1);
    fs::path p = current / buffer;

    if (!fs::exists(p) || !fs::is_regular_file(p))
    {
        socket.sendData( &ERROR, sizeof(ERROR));
        res = "retri failed: File doesn't exist or couldn't be opened";
        socket.sendData( res.c_str(), res.length());
    }
    else
    {
        socket.sendData( &OK, sizeof(OK));

        u_int32_t lengh = fs::file_size(p);
        socket.sendData( &lengh, sizeof(lengh));
        std::ifstream myfile;
        myfile.open(p);
        while (lengh > 0)
        {
            myfile.read(buffer, std::min(BUFFER_SIZE, lengh));
            socket.sendData( buffer, myfile.gcount());
            lengh -= myfile.gcount();
        }
        myfile.close();
    }
}

void stor(Socket& socket, fs::path& current) {
    char buffer[4096] = {};
    uint32_t lengh;
    uint32_t name_size;
    socket.recvData( &name_size, sizeof(name_size));
    socket.recvData( buffer, name_size);

    std::ofstream myfile;
    fs::path p = current / buffer;

    myfile.open(p);
    socket.recvData( &lengh, sizeof(lengh));

    myfile.clear();
    while (lengh > 0)
    {
        auto bytes = socket.recvData( &buffer, std::min(BUFFER_SIZE, lengh));
        myfile.write(buffer, bytes);
        lengh -= bytes;
    }

    myfile.close();
}
#endif //DOWNLOAD_UPLOAD_H
