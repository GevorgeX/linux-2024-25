#pragma once
#include <vector>
#include <dirent.h>
#include <memory>
#include <queue>
#include <string>
#include <memory>
#include <stdexcept>

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

    void parse_childs();

    explicit Directory(std::string name, std::string parent_dir, DirType type);

public:
    explicit Directory(std::string name);

    const std::string& get_name() const;
    const std::string get_path() const;
    const Directory& operator[](const unsigned i) const;
    unsigned size() const;
    DirType get_type() const;

    class RecursiveDirectoryIterator {
        std::queue<Directory*> queue;

    public:
        RecursiveDirectoryIterator();
        explicit RecursiveDirectoryIterator(Directory* root);

        std::string operator*();
        RecursiveDirectoryIterator& operator++();
        bool operator!=(const RecursiveDirectoryIterator& other) const;
    };

    RecursiveDirectoryIterator begin();
    RecursiveDirectoryIterator end();
};
