#include "Cp.h"
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <vector>

const std::string Cp::getFileName(const std::string& s) {
    auto pos = s.find_last_of("/");
    if (pos == std::string::npos) {
        return s; // No separator found, so path is the filename
    } else {
        return s.substr(pos + 1); // Extract filename after last separator
    }
}

void Cp::copy_file(const std::string& from, const std::string& to, bool no_override, bool force) {
    int fd_from = open(from.c_str(), O_RDONLY);
    if (fd_from == -1) {
        std::cerr << "open failed" << std::endl;
        return;
    }

    int fd_to = open(to.c_str(), O_WRONLY | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);
    if (fd_to == -1) {
        std::cerr << "open/create failed" << std::endl;
    }

    struct stat st{};
    fstat(fd_from, &st);
    auto size = st.st_size;
    std::vector<char> buff(size);
    read(fd_from, buff.data(), size);

    write(fd_to, buff.data(), size);

    close(fd_from);
    close(fd_to);
}

void Cp::copy_dir(const Directory* dir, const std::string& to, bool no_override, bool force) {
    mkdir(to.c_str(), S_IRWXU | S_IRGRP | S_IROTH);

    for (int i = 0; i < dir->size(); i++) {
        auto t = &dir->operator[](i);

        if (t->get_type() == Directory::File) {
            copy_file(t->get_path(), to + "/" + t->get_name(), no_override, force);
        }
        else {
            // Handle subdirectory copy here if needed
            // copy_dir(t, to + "/" + t->get_name(), no_override, force);
        }
    }
}

void Cp::copy(const std::string& from, const std::string& to, bool recurs, bool no_override, bool force) {
    if (recurs) {
        Directory dir(from);
        copy_dir(&dir, to, no_override, force);
        return;
    }

    auto fd = opendir(to.c_str());

    if (fd == nullptr) {
        copy_file(from, to, no_override, force);
    }
    else {
        copy_file(from, to + "/" + getFileName(from), no_override, force);
        closedir(fd);
    }
}
