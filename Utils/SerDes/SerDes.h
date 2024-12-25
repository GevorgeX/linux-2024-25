#pragma once
#include <string>
#include <unistd.h>
#include <filesystem>

#define MAGIC_NUMBER static_cast<std::byte>('R' + 'O' + 'B')

class SmartMap {
    std::byte* data;
    size_t length;

public:
    SmartMap(void* addr, size_t length, int prot, int flags, int fd, off_t offset);
    ~SmartMap();
    std::byte* get() const;
};

class SmartOpen {
    int fd;

public:
    SmartOpen(const std::string& name, int prot, int flags = 0);
    ~SmartOpen();
    int get() const;
};

template<class T>
void serialization(const std::filesystem::path& file, const T& obj);

template<class T>
void deserialization(const std::filesystem::path& file, T& obj);


//FOR DEBUG
struct ST {
    unsigned int a;
    long long b;
    char c;

    ST() = default;
    ST(unsigned int a, long long b, char c) : a(a), b(b), c(c) {}
};

struct Mec {
    char m_data[1024*1024];
};