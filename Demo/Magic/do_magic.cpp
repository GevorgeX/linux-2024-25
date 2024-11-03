#include <unistd.h>
#include <fcntl.h>

void do_magic()
{
    int t = open("TestFiles/new_pts.txt", O_RDONLY);
    dup2(t,0);
}
