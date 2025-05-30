#ifndef DIRECTION_H
#define DIRECTION_H

namespace fs = std::filesystem;

void list(Socket& socket, fs::path& current) {
    std::string res;
    for (const auto &entry : fs::directory_iterator(current))
    {
        res.append(entry.path().filename().string());
        res.append("\n");
    }

    socket.sendData( res.c_str(), res.length());
}



void pwd(Socket& socket, fs::path& current) {
    std::string res;
    res = current.string();
    socket.sendData( res.c_str(), res.length());
}

void cwd(Socket& socket, fs::path &current) {
    char buffer[4096] = {};
    std::string res;
    socket.recvData( buffer, BUFFER_SIZE - 1);
    auto p = current / buffer;

    if (!fs::exists(p) || !fs::is_directory(p))
    {
        socket.sendData( &ERROR, sizeof(ERROR));
        res = "cdw failed: Path does not exist or is not a directory";
        socket.sendData( res.c_str(), res.length());
    }
    else
    {
        socket.sendData( &OK, sizeof(OK));
        current = p;
    }
}

void rmd(Socket& socket, fs::path& current) {
    char buffer[4096] = {};
    std::string res;
    socket.recvData( buffer, BUFFER_SIZE - 1);
    auto p = current / buffer;

    if (!fs::remove_all(p))
    {
        socket.sendData( &ERROR, sizeof(ERROR));
        res = "rmd failed: Directory not empty or doesn't exist";
        socket.sendData( res.c_str(), res.length());
    }
    else
    {
        socket.sendData( &OK, sizeof(OK));
    }
}

void mkd(Socket& socket, fs::path& current) {
    char buffer[4096] = {};
    std::string res;
    socket.recvData( buffer, BUFFER_SIZE - 1);
    auto p = current / buffer;

    if (!fs::create_directory(p))
    {
        socket.sendData( &ERROR, sizeof(ERROR));
        res = "mkdir failed: Could not create directory (already exists?)";
        socket.sendData( res.c_str(), res.length());
    }
    else
    {
        socket.sendData( &OK, sizeof(OK));
    }
}

void dele(Socket& socket, fs::path& current) {
    char buffer[4096] = {};
    std::string res;
    socket.recvData( buffer, BUFFER_SIZE - 1);
    auto p = current / buffer;
    if (!fs::remove(p))
    {
        socket.sendData( &ERROR, sizeof(ERROR));
        res = "delete failed: File doesn't exist or couldn't be deleted";
        socket.sendData( res.c_str(), res.length());
    }
    else
    {
        socket.sendData( &OK, sizeof(OK));
    }
}
#endif //DIRECTION_H
