#pragma once
#include <unistd.h>
#include <sys/wait.h>

void DoCommand(const std::string& cmd){
    auto child = fork();
    if (child == 0)
    {
        int ret = execl("/bin/sh","sh" ,"-c", cmd.c_str(), (char*)NULL);
        if (ret < 0)
        {
            perror(nullptr);
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    }
    else
    {
        int ret;
        wait(&ret);
        if(WIFEXITED(ret))
        {
            std::cout << "Process " << child << " exited with exit code " << WEXITSTATUS(ret) << ".\n";
        }
        else if (WIFSIGNALED(ret))
        {
            std::cout << "Process " << child << " was killed with signal " << WTERMSIG(ret) << ".\n";
        }
        exit(EXIT_SUCCESS);
    }
}
