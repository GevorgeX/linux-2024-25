#include <iostream>
#include <csignal>
#include <fstream>
#include <random>
#include <unistd.h>
#include <algorithm>

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

    std::vector<bool> procs(proc_count-spy_count, false);
    for (int j = 0; j < spy_count; ++j)
        procs.push_back(true);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(procs.begin(), procs.end(), g);

    for (auto is_spy : procs)
    {
        pid_t pid = fork();
        if(pid == 0)
        {
            if( is_spy) //spy
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

            while (true){}

        }
        else if (pid > 0)
        {
            MyFile << pid <<std::endl;
        }
        else
        {
            std::cerr << "Fork failed" << std::endl;
        }
    }
    MyFile.close();
    while (true){}
}
