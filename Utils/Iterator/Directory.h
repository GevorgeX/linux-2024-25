#pragma once
#include <vector>
#include <dirent.h>
#include <queue>

class Directory{

    std::string name;
    std::vector<Directory> files;
    Directory* parent;
    bool is_folder;
    std::string get_path() const
    {
        return (parent == nullptr ? "" : parent->get_path()) + "/" + name;
    }
public:
    Directory(std::string name, Directory* parent = nullptr, bool is_folder = true): parent(parent), name(name),is_folder(is_folder)
    {
        auto dir = opendir(get_path().c_str());
        if(dir == nullptr)
        {
            this->is_folder = false;
            return;
        }
        dirent* file;
        readdir(dir);
        readdir(dir);
        while ((file  = readdir(dir)) != nullptr)
        {
            files.emplace_back(file->d_name, this);
        }
        closedir(dir);
    }
    class RecursiveDirectoryIterator {
        std::queue<Directory*> queue;

    public:
        RecursiveDirectoryIterator(Directory* root) {
            if (root) {
                queue.push(root);
            }
        }

        Directory& operator*() {
            while (queue.front()->is_folder && !queue.empty())
            {
                ++(*this);
            }
            return *queue.front();
        }

        RecursiveDirectoryIterator& operator++() {
            if (queue.empty()) return *this;

            Directory* current = queue.front();
            queue.pop();
            for (auto& child : current->files) {
                queue.push(&child);
            }
            return *this;
        }
        bool operator!=(const RecursiveDirectoryIterator& other) const {
            return queue != other.queue;
        }
    };

    RecursiveDirectoryIterator begin() {
        return RecursiveDirectoryIterator(this);
    }

    RecursiveDirectoryIterator end() {
        return RecursiveDirectoryIterator(nullptr);
    }

    std::string get_name(){return name;}
};
