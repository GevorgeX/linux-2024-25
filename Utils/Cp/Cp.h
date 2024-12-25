#pragma once
#include <string>
#include <vector>
#include "../Directory/Directory.h"

class Cp {
private:
    static const std::string getFileName(const std::string& s);
    static void copy_file(const std::string& from, const std::string& to, bool no_override, bool force);
    static void copy_dir(const Directory* dir, const std::string& to, bool no_override, bool force);

public:
    static void copy(const std::string& from, const std::string& to, bool recurs, bool no_override, bool force);
};
