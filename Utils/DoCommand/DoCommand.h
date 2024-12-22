#pragma once
#include <unistd.h>
#include <sys/wait.h>

void DoCommand(const std::string& cmd){
    auto child = fork();
    if (child == 0)
    {
        execl("/bin/sh","sh" ,"-c", cmd.c_str(), nullptr);
    }
    else
    {
        int status;
        waitpid(child, &status, 0)
    }
}
