#pragma once
#include <iostream>
#include <string>
#include "../Directory/Directory.h"
#include "../ArgumentParser/ArgumentParser.h"

class SuperRm {
private:
    static void clear(const std::string& name);
    static void remove_file(const std::string& name, bool debug);
    static void remove_folder(const std::string& name, bool debug);
    static void remove_dir(const Directory& dir, bool debug_mode, bool recur);

public:
    static void remove(int argc, char** argv);
};
