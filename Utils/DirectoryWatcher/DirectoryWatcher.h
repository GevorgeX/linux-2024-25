#pragma once
#include <sys/inotify.h>
#include <fstream>

#include <Directory/Directory.h>

#define MAX_EVENTS 128
#define LEN_NAME 64
#define EVENT_SIZE ( sizeof (struct inotify_event) )
#define BUF_LEN ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME ))

class DirectoryWatcher {
    int m_fd;

    std::ofstream log;

    void parseChildren(const Directory& dir) {
        unsigned mask;

        if(dir.get_type() == Directory::Folder) {
            mask = IN_CREATE | IN_DELETE;
        }
        else {
            mask = IN_MODIFY;
        }
        auto t = inotify_add_watch(m_fd, dir.get_path().c_str(), mask);
        if (t == -1) {
            throw std::runtime_error( "cant add file/forlder");
        }

        auto size = dir.size();
        for (int i = 0; i < size; i++) {
            parseChildren(dir[i]);
        }
    }

public:
    explicit DirectoryWatcher(const std::string& path, const std::string& log_path) {
        log.open(log_path);
        m_fd = inotify_init1(0);
        if (m_fd < 0) {
            throw std::runtime_error( "Cant create inotify object");
        }

        Directory root(path);
        auto t = inotify_add_watch(m_fd, path.c_str(), IN_MODIFY);
        if (t == -1) {
            throw std::runtime_error( "cant add file/forlder");
        }
        parseChildren(root);

    }
    ~DirectoryWatcher() {
        close(m_fd);
        log.close();
    }

    void watch() {
        char buffer[BUF_LEN]
     __attribute__ ((aligned(__alignof__(struct inotify_event))));
        const struct inotify_event *event;

        while(true)
        {
            long int len = read(m_fd, buffer, sizeof buffer);

            if (len == -1 ) {
                throw std::runtime_error( "Cant read from inotify object");
            }

            if (len <= 0)
                break;

            for (char *ptr = buffer; ptr < buffer + len;
                    ptr += sizeof(struct inotify_event) + event->len) {
                event = reinterpret_cast<struct inotify_event *>(ptr);


                if ((event->mask & IN_CREATE) != 0) {
                    log<<"Created: "<< event->name << std::endl;
                }
                if ((event->mask & IN_DELETE) != 0) {
                    log<<"Deleted: "<< event->name << std::endl;
                }
                if ((event->mask & IN_MODIFY) != 0) {
                    log<<"Modify: "<< event->name << std::endl;
                }
            }
        }
    }
};