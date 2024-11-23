#pragma once
#include <fcntl.h>
#include <string>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstring>
#include <filesystem>

#define MAGIC_NUMBER ('R' + 'O' + 'B')
class SmartMap {
    unsigned char* data;
    size_t length;
    public:
    SmartMap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset):length(length) {
        auto t = mmap(addr, length, prot, flags, fd, offset);
        if (t == MAP_FAILED) {
            close(fd);
            throw std::runtime_error("Failed to mmap file");
        }
        data = static_cast<unsigned char*>(t);
    }
    ~SmartMap() {
        munmap(data, length);
    }
    unsigned char* get() const {
        return data;
    }
};

template<class T>
void serialization(const std::filesystem::path& file, const T& obj ) {
    static_assert(std::is_trivially_copyable_v<T>);

    int fd = open(file.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        throw std::runtime_error("Failed to open file");
    }

    constexpr size_t length = sizeof(T);
    if (ftruncate(fd, length+1) == -1) {
        close(fd);
        throw std::runtime_error("Failed to resize file");
    }

    SmartMap t(nullptr, length+1,  PROT_WRITE, MAP_SHARED, fd, 0);
    *t.get() = MAGIC_NUMBER;
    std::memcpy(t.get()+1, &obj, length);
    close(fd);
}

template<class T>
void deserialization(const std::filesystem::path& file, T& obj) {
    static_assert(std::is_trivially_copyable_v<T>);

    int fd = open(file.c_str(), O_RDONLY);
    if (fd == -1) {
        throw std::runtime_error("Failed to open file");
    }
    constexpr size_t length = sizeof(T);

    SmartMap t(nullptr,length+1, PROT_READ, MAP_PRIVATE,fd, 0);

    if(*t.get() != MAGIC_NUMBER) {
        throw std::runtime_error("Failed to deserialize file");
    }
    std::memcpy(&obj, t.get()+1, length);

    close(fd);
}
