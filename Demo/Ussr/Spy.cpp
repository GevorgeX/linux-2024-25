#include <iostream>
#include <csignal>
#include <fstream>
#include <random>
#include <unistd.h>
#include <algorithm>
#include <thread>
#include <sys/wait.h>

int main()
{
    constexpr int proc_count = 30;
    constexpr int spy_count = 10;

    std::ofstream MyFile("postman.txt");
    MyFile.clear();

    std::vector<std::pair<bool,int>> procs(proc_count-spy_count, std::make_pair(false,0));
    for (int j = 0; j < spy_count; ++j)
        procs.emplace_back(true,0);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(procs.begin(), procs.end(), g);

    struct sigaction sa{};
    sa.sa_sigaction = [](int, siginfo_t *info, void*)
    {
        std::cout<<"HI HITLER\n";
        kill(info->si_pid, SIGILL);
    };
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    for (auto &prc : procs)
    {
        pid_t pid = fork();
        if(pid == 0)
        {
            if( prc.first) //spy
            {
                sigaction(SIGINT, &sa, nullptr);
            }

            while (true);

        }
        if (pid > 0)
        {
            prc.second = pid;
            MyFile << pid <<std::endl;
        }
        else
        {
            std::cerr << "Fork failed" << std::endl;
        }
    }
    MyFile.close();
    int status;
    for (auto prc : procs) {
        if (prc.first)
            waitpid(prc.second,&status,0);
    }

    for (auto prc : procs) {
        kill(prc.second, SIGTERM);
    }
}
