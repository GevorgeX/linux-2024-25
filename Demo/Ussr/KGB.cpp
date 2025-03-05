#include <fstream>
#include <iostream>
#include <csignal>

int main()
{
    std::ifstream MyFile("postman.txt");
    std::string s;

    struct sigaction sa{};
    sa.sa_sigaction = [](int, siginfo_t *info, void*)
    {
        std::cout<<"BRNVAR ARA " << info->si_pid <<"\n";
        kill(info->si_pid, SIGTERM);
    };

    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGILL, &sa, nullptr);

    struct sigaction ss{};
    ss.sa_sigaction = [](int, siginfo_t *, void*)
    {};
    ss.sa_flags = SA_SIGINFO;
    sigemptyset(&ss.sa_mask);
    sigaction(SIGINT, &ss, nullptr);

    while (getline(MyFile, s))
    {
        int num = std::stoi(s);
        kill(num,SIGINT);
        pause();
    }
    MyFile.close();
}
