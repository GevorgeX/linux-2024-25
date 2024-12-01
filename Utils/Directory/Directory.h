#pragma once
#include <vector>
#include <dirent.h>
#include <memory>
#include <queue>
#include <string>

class Directory {
public:
    enum DirType {
        Folder,
        File
    };

private:
    DirType type;
    std::string parent_dir;
    std::string name;
    std::vector<std::unique_ptr<Directory>> childs;

    void parse_childs()
    {
        auto dir = opendir(get_path().c_str());
        dirent* file;
        while ((file = readdir(dir)) != nullptr) {
            if (file->d_name == std::string_view(".") ||file->d_name == std::string_view("..")) {
                continue;
            }

            auto type_dir = (file->d_type == DT_DIR) ? DirType::Folder : DirType::File;
            childs.push_back(std::make_unique<Directory>(std::move(Directory(file->d_name, get_path(), type_dir))));
        }
        closedir(dir);
    }

    explicit Directory(std::string  name ,std::string  parent_dir, DirType type)
    : type(type), name(std::move(name)), parent_dir(std::move(parent_dir))
    {
        if (type == DirType::File) {
            return;
        }

        parse_childs();
    }

public:
    explicit Directory(std::string name)
        :name(std::move(name)), parent_dir(".")
    {
        auto dir = opendir(get_path().c_str());

        if(dir == nullptr)
        {
            if(errno == ENOENT)
            {
                throw std::runtime_error(get_path()+" does not exist");
            }
            type = DirType::File;
        }
        else
        {
            type = DirType::Folder;
            parse_childs();
        }

    }
    const std::string& get_name() const {
        return name;
    }

    const std::string get_path() const
    {
        return parent_dir+"/"+name;
    }
    const Directory& operator[](const unsigned i) const
    {
        return *childs[i];
    }

    unsigned size() const
    {
        return (type == Folder) ? childs.size() : 0;
    }

    DirType get_type() const
    {
        return type;
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
