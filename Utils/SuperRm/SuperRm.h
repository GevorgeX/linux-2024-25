#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <Directory/Directory.h>
#include <ArgumentParser/ArgumentParser.h>

class SuperRm {
    static void clear(const std::string& name) {
        int fd = open(name.c_str(), O_WRONLY);
        struct stat st{};
        fstat(fd, &st);
        auto size = st.st_size;

        int buffer_size = 4096;
        const std::string buf(buffer_size, '0');

        for (off_t t = 0; t < size; t+= buffer_size )
        {
            write(fd, buf.c_str(), buffer_size);
        }
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

    static void remove_dir(const Directory& dir, bool debug_mode, bool recur) {
        switch (dir.get_type()) {
            case Directory::File:
                clear(dir.get_path());
                remove_file(dir.get_path(), debug_mode);
                break;

            case Directory::Folder:
                if(recur)
                {
                    for (int i = 0; i < dir.size(); i++) {
                        remove_dir(dir[i], debug_mode, true);
                    }
                }
                remove_folder(dir.get_path(), debug_mode);
                break;
        }
    }

public:
    static void remove(int argc, char** argv) {
        ArgumentParser arguments(argc, argv);
        arguments.parse("rv");

        std::string name(argv[optind]);
        bool r = false;
        bool v = false;
        Directory dir(name);

        for (auto &t : arguments)
        {
            if(t.flag == 'r')
                r = true;
            if(t.flag == 'v')
                v = true;
        }


        switch (dir.get_type()) {
            case Directory::File:
                clear(dir.get_path());
                remove_file(dir.get_path(), v);
                break;

            case Directory::Folder:
                remove_dir(dir, v ,r);
                break;
        }

    }
};
