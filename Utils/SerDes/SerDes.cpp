#include "SerDes.h"
#include <stdexcept>
#include <iostream>
#include <filesystem>
#include <sys/mman.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>

SmartMap::SmartMap(void* addr, size_t length, int prot, int flags, int fd, off_t offset)
    : length(length) {
    auto t = mmap(addr, length, prot, flags, fd, offset);
    if (t == MAP_FAILED) {
        throw std::runtime_error("Failed to mmap file");
    }
    data = static_cast<std::byte*>(t);
}

SmartMap::~SmartMap() {
    munmap(data, length);
}

std::byte* SmartMap::get() const {
    return data;
}

SmartOpen::SmartOpen(const std::string& name, int prot, int flags) {
    fd = open(name.c_str(), prot, flags);

    if (fd == -1) {
        throw std::runtime_error("Failed to open file");
    }
}

SmartOpen::~SmartOpen() {
    close(fd);
}

int SmartOpen::get() const {
    return fd;
}

template<class T>
void serialization(const std::filesystem::path& file, const T& obj) {
    static_assert(std::is_trivially_copyable_v<T>);

    SmartOpen reader(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    constexpr size_t length = sizeof(T);
    if (ftruncate(reader.get(), length + 1) == -1) {
        throw std::runtime_error("Failed to resize file");
    }

    SmartMap t(nullptr, length + 1, PROT_WRITE, MAP_SHARED, reader.get(), 0);
    *t.get() = MAGIC_NUMBER;
    std::memcpy(t.get() + 1, &obj, length);
}

template<class T>
void deserialization(const std::filesystem::path& file, T& obj) {
    static_assert(std::is_trivially_copyable_v<T>);

    SmartOpen reader(file.c_str(), O_RDONLY);
    if (reader.get() == -1) {
        throw std::runtime_error("Failed to open file");
    }
    constexpr size_t length = sizeof(T);

    SmartMap t(nullptr, length + 1, PROT_READ, MAP_PRIVATE, reader.get(), 0);

    if (*t.get() != MAGIC_NUMBER) {
        throw std::runtime_error("Failed to deserialize file, Your file isn't valid");
    }
    std::memcpy(&obj, t.get() + 1, length);
}

template void serialization<ST>(const std::filesystem::path&, const ST&);
template void deserialization<ST>(const std::filesystem::path&, ST&);
template void serialization<Mec>(const std::filesystem::path&, const Mec&);
template void deserialization<Mec>(const std::filesystem::path&, Mec&);
