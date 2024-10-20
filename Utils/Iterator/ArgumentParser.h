#pragma once
#include <optional>
#include <string>
#include <unistd.h>
#include <vector>

class Argument
{
public:
    char flag;
    std::optional<std::string> value;
    Argument(char flag, std::optional<std::string> value = {}) : flag(flag), value(value) {}
};

class ArgumentParser
{
    int argc;
    char **argv;
    std::vector<Argument> arguments;
    public:
    ArgumentParser(int argc, char** argv):argc(argc), argv(argv){}
    void parse(const char *opstr)
    {
        opterr = 0;
        int rez;
        while ((rez = getopt(argc, argv, opstr)) != -1){

            if(rez == '?')
                throw std::invalid_argument("Invalid option: "+ static_cast<char>(rez));

            if(optarg != nullptr)
            {
                arguments.emplace_back(rez, std::string(optarg));
            }
            else
            {
                arguments.emplace_back(rez);
            }

        } 
    }
    class Iterator
    {
        Argument* arr;
    public:
        Iterator(Argument* arr):arr(arr){}
        Argument* operator->() const {return arr;}
        Argument& operator*() const {return *arr;}
        Iterator operator++()
        {
            arr++;
            return *this;
        }
        Iterator operator++(int)
        {
            Iterator tmp = *this;
            arr++;
            return tmp;
        }
        bool operator!=(const Iterator& s) const{return arr !=s.arr;}

    };
    Iterator begin()
    {
        return Iterator(arguments.data());
    }
    Iterator end()
    {
        return Iterator(arguments.data() + arguments.size());
    }
};