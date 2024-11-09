#include <cstdio>
#include <unistd.h>
#include <fcntl.h>

void do_magic()
{
    int t = open("TestFiles/new_pts.txt", O_RDONLY);
    if (t == -1) {
        perror("open");
    }
    dup2(t,0);
}
