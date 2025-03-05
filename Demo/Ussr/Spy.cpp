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
    int proc_count;
    std::cout << "Select proces count: " << std::endl;
    std::cin >> proc_count;
    int spy_count;
    std::cout << "Select spy count: " << std::endl;
    std::cin >> spy_count;

    std::ofstream MyFile("postman.txt");
    MyFile.clear();

    std::vector<std::pair<bool,int>> procs(proc_count-spy_count, std::make_pair(false,0));
    for (int j = 0; j < spy_count; ++j)
        procs.emplace_back(true,0);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(procs.begin(), procs.end(), g);

    for (auto &prc : procs)
    {
        pid_t pid = fork();
        if(pid == 0)
        {
            if( prc.first) //spy
            {
                struct sigaction sa{};
                sa.sa_sigaction = [](int, siginfo_t *info, void*)
                {
                    std::cout<<"HI HITLER\n";
                    kill(info->si_pid, SIGILL);
                };
                sa.sa_flags = SA_SIGINFO;
                sigemptyset(&sa.sa_mask);
                sigaction(SIGINT, &sa, nullptr);
            }
            else {
                struct sigaction sa{};
                sa.sa_sigaction = [](int, siginfo_t *info, void*)
                {
                    std::cout<<"Axper es lav txa em\n";
                    kill(info->si_pid, SIGINT);
                };
                sa.sa_flags = SA_SIGINFO;
                sigemptyset(&sa.sa_mask);
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

    std::this_thread::sleep_for(std::chrono::seconds(2));
    for (auto prc : procs) {
        kill(prc.second, SIGTERM);
    }
}
