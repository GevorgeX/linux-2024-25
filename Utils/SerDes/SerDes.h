#pragma once
#include <fcntl.h>
#include <string>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstring>

template<class T>
void serialization(const std::string& file, const T& obj ) {
    int fd = open(file.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        throw std::runtime_error("Failed to open file");
    }

    size_t length = sizeof(T);
    if (ftruncate(fd, length) == -1) {
        close(fd);
        throw std::runtime_error("Failed to resize file");
    }

    void* t = mmap(nullptr, length,  PROT_WRITE, MAP_SHARED, fd, 0);
    if (t == MAP_FAILED) {
        close(fd);
        throw std::runtime_error("Failed to mmap file");
    }

    std::memcpy(t, &obj, length);

    munmap(t, length);
    close(fd);
}

template<class T>
void deserialization(const std::string& file, T& obj) {
    int fd = open(file.c_str(), O_RDONLY);
    if (fd == -1) {
        throw std::runtime_error("Failed to open file");
    }

    size_t length = sizeof(T);

    auto t = mmap(nullptr,length, PROT_READ, MAP_PRIVATE,
                  fd, 0);

    if (t == MAP_FAILED) {
        close(fd);
        throw std::runtime_error("Failed to mmap file");
    }

    std::memcpy(&obj, t, length);

    munmap(t, length);
    close(fd);
}
