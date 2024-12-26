#pragma once
#include "DoCommand/DoCommand.h"

void xargs(int argc, char* argv[]) {

    std::string input;
    std::string in;
    while (std::getline(std::cin, input)) {
        in.append(input[0]);
    }
    std::string args;
    for (int i=1;i<argc;i++)
        args.append(std::string(argv[i]).append(" "));

    args.append(in);
    DoCommand(args);
}
