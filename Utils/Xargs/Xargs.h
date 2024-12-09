#pragma once
#include "DoCommand/DoCommand.h"

void xargs(int argc, char* argv[]) {

    std::string input;
    std::getline(std::cin, input);

    std::string args;
    for (int i=1;i<argc;i++)
        args.append(std::string(argv[i]).append(" "));

    args.append(input);
    DoCommand(args);
}
