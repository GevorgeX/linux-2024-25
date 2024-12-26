#pragma once
#include <fstream>
#include <filesystem>


#define MAX_EVENTS 128
#define LEN_NAME 64
#define EVENT_SIZE ( sizeof (struct inotify_event) )
#define BUF_LEN ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME ))

class DirectoryWatcher {
    int m_fd;
    std::ofstream log;

public:
    explicit DirectoryWatcher(const std::filesystem::path& path, const std::filesystem::path& log_path);
    ~DirectoryWatcher();

    void watch();
};