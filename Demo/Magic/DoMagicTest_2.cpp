#include <unistd.h>
#include <fcntl.h>
#include <cstring>

int main()
{
    int t = open("TestFiles/exclusive_file.log", O_WRONLY);
    int t2 = dup(t);
    const char* fline = "First Line\n";
    const char* sline = "Second Line\n";
    write(t, fline, strlen(fline));
    write(t2, sline, strlen(sline));

    close(t);
    close(t2);
}