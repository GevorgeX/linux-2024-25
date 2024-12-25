#pragma once
#include <optional>
#include <string>
#include <unistd.h>
#include <vector>

class Argument {
public:
    char flag;
    std::optional<std::string> value;

    explicit Argument(char flag, std::optional<std::string> value = {});
};

class ArgumentParser {
    int argc;
    char** argv;
    std::vector<Argument> arguments;

public:
    ArgumentParser(int argc, char** argv);

    void parse(const char* opstr);

    class Iterator {
        Argument* arr;

    public:
        explicit Iterator(Argument* arr);

        Argument* operator->() const;
        Argument& operator*() const;
        Iterator& operator++();
        Iterator operator++(int);
        bool operator!=(const Iterator& other) const;
    };

    Iterator begin();
    Iterator end();
};
