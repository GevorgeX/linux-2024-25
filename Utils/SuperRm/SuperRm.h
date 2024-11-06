#pragma once
#include <iostream>
#include <stack>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "Directory/Directory.h"

class SuperRm {
    static void clear(const std::string& name) {
        int fd = open(name.c_str(), O_WRONLY);
        struct stat st{};
        fstat(fd, &st);
        auto size = st.st_size;
        const std::string buf(size, '0');
        write(fd, buf.c_str(), size);
        close(fd);
    }

    static void remove_file(const std::string& name, bool debug) {
        if (unlink(name.c_str()) == -1) {
            std::cerr << "Error removing " << name << "\n";
            return;
        }

        if (debug)
            std::cout << "Removing " << name << "\n";
    }

    static void remove_folder(const std::string& name, bool debug) {
        if (rmdir(name.c_str()) == -1) {
            std::cerr << "Error removing " << name << "\n";
            return;
        }

        if (debug)
            std::cout << "Removing " << name << "\n";
    }

    static void remove_dir(const Directory& dir, bool debug_mode) {
        switch (dir.get_type()) {
            case Directory::File:
                clear(dir.get_path());
                remove_file(dir.get_path(), debug_mode);
                break;

            case Directory::Folder:
                for (int i = 0; i < dir.size(); i++) {
                    remove_dir(dir[i], debug_mode);
                }
                remove_folder(dir.get_path(), debug_mode);
                break;
        }
    }

public:
    static void remove(const std::string& name, bool remove_dirs = false, bool debug_mode = false) {
        Directory dir(name);
        if (remove_dirs) {
            remove_dir(dir, debug_mode);
        } else {
            switch (dir.get_type()) {
                case Directory::File:
                    clear(dir.get_path());
                    remove_file(dir.get_path(), debug_mode);
                    break;

                case Directory::Folder:
                    remove_folder(dir.get_path(), debug_mode);
                    break;
            }
        }
    }
};
