#pragma once
#include <fcntl.h>
#include <string>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <cstring>

template<class T>
void serialization(const std::string& file, const T* obj ) {
    int fd = open(file.c_str(), O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

    auto bytes = reinterpret_cast<const char*>(obj);
    write(fd, bytes, sizeof(T));

    close(fd);
}

template<class T>
void deserialization(const std::string& file, T* obj) {
    int fd = open(file.c_str(), O_RDONLY);
    struct stat st{};
    fstat(fd, &st);

    auto t = mmap(0, st.st_size, PROT_READ, MAP_SHARED,
        fd, 0);

    if (t == MAP_FAILED) {
        perror("mmap");
        return;
    }

    std::memcpy(obj, t, st.st_size);

    munmap(t, st.st_size);
    close(fd);
}
