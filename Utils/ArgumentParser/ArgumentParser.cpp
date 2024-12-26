#include "ArgumentParser.h"
#include <stdexcept>
#include <getopt.h>  // for getopt()

Argument::Argument(char flag, std::optional<std::string> value)
    : flag(flag), value(std::move(value)) {}

ArgumentParser::ArgumentParser(int argc, char** argv)
    : argc(argc), argv(argv) {}

void ArgumentParser::parse(const char* opstr) {
    opterr = 0;
    int rez;
    while ((rez = getopt(argc, argv, opstr)) != -1) {
        if (rez == '?') {
            throw std::invalid_argument("Invalid option: " + std::string(1, static_cast<char>(rez)));
        }

        if (optarg != nullptr) {
            arguments.emplace_back(rez, std::string(optarg));
        } else {
            arguments.emplace_back(rez);
        }
    }
}

ArgumentParser::Iterator::Iterator(Argument* arr)
    : arr(arr) {}

Argument* ArgumentParser::Iterator::operator->() const {
    return arr;
}

Argument& ArgumentParser::Iterator::operator*() const {
    return *arr;
}

ArgumentParser::Iterator& ArgumentParser::Iterator::operator++() {
    arr++;
    return *this;
}

ArgumentParser::Iterator ArgumentParser::Iterator::operator++(int) {
    Iterator tmp = *this;
    arr++;
    return tmp;
}

bool ArgumentParser::Iterator::operator!=(const Iterator& other) const {
    return arr != other.arr;
}

ArgumentParser::Iterator ArgumentParser::begin() {
    return Iterator(arguments.data());
}

ArgumentParser::Iterator ArgumentParser::end() {
    return Iterator(arguments.data() + arguments.size());
}
