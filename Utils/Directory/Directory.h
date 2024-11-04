#pragma once
#include <vector>
#include <dirent.h>
#include <queue>
#include <string>

class Directory {
    enum DirType {
        Folder,
        File
    } type;

    std::string name;
    std::vector<std::unique_ptr<Directory>> childs;
    Directory* parent;

public:
    explicit Directory(std::string name, DirType type = Folder, Directory* parent = nullptr)
        : type(type), name(std::move(name)), parent(parent)
    {
        if (type == DirType::File) {
            return;
        }

        auto dir = opendir(get_path().c_str());
        dirent* file;
        while ((file = readdir(dir)) != nullptr) {
            if (file->d_name == std::string_view(".") ||file->d_name == std::string_view("..")) {
                continue;
            }

            auto type_dir =  (file->d_type == DT_DIR) ? DirType::Folder : DirType::File;
            childs.push_back(std::make_unique<Directory>(file->d_name, type_dir, this));
        }
        closedir(dir);
    }

    std::string get_path() const {
        if (parent != nullptr) {
            return parent->get_path() + "/" + name;
        } else {
            return name;
        }
    }

    std::string get_name() const {
        return name;
    }

    class RecursiveDirectoryIterator {
        std::queue<Directory*> queue;

    public:
        RecursiveDirectoryIterator() = default;

        explicit RecursiveDirectoryIterator(Directory* root) {
            queue.push(root);
        }

        std::string operator*() {
            while (queue.front()->type == Folder) {
                ++(*this);
            }
            return queue.front()->name;
        }

        RecursiveDirectoryIterator& operator++() {
            auto current = queue.front();
            queue.pop();
            if (current->type == DirType::Folder) {
                for (auto &child : current->childs) {
                    queue.push(child.get());
                }
            }
            return *this;
        }

        bool operator!=(const RecursiveDirectoryIterator& other) const {
            return queue != other.queue;
        }
    };

    RecursiveDirectoryIterator begin() {
        return RecursiveDirectoryIterator{this};
    }

    RecursiveDirectoryIterator end() {
        return RecursiveDirectoryIterator{};
    }
};
