#include "DirectoryWatcher.h"
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <sys/inotify.h>

#include "../Directory/Directory.h"

DirectoryWatcher::DirectoryWatcher(const std::filesystem::path& path, const std::filesystem::path& log_path) {
    log.open(log_path);
    if (!log.is_open()) {
        throw std::runtime_error("Failed to open log file");
    }

    m_fd = inotify_init1(0);
    if (m_fd < 0) {
        throw std::runtime_error("Can't create inotify object");
    }

    Directory root(path);
    auto t = inotify_add_watch(m_fd, path.c_str(), IN_MODIFY | IN_CREATE | IN_DELETE);
    if (t == -1) {
        throw std::runtime_error("Can't add file/folder to inotify watch list");
    }
}

DirectoryWatcher::~DirectoryWatcher() {
    close(m_fd);
    log.close();
}

void DirectoryWatcher::watch() {
    char buffer[BUF_LEN];
    const struct inotify_event* event;

    while (true) {
        long int len = read(m_fd, buffer, sizeof(buffer));

        if (len == -1) {
            throw std::runtime_error("Can't read from inotify object");
        }

        if (len <= 0) {
            break;
        }

        for (char* ptr = buffer; ptr < buffer + len; ptr += sizeof(struct inotify_event) + event->len) {
            event = reinterpret_cast<const struct inotify_event*>(ptr);

            if ((event->mask & IN_CREATE) != 0) {
                log << "Created: " << event->name << std::endl;
            }
            if ((event->mask & IN_DELETE) != 0) {
                log << "Deleted: " << event->name << std::endl;
            }
            if ((event->mask & IN_MODIFY) != 0) {
                log << "Modified: " << event->name << std::endl;
            }
        }
    }
}
