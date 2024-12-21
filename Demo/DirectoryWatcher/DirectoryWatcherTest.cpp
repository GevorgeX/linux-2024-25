#include <iostream>
#include <DirectoryWatcher/DirectoryWatcher.h>


int main(int argc, char** argv) {
    DirectoryWatcher wathcer(argv[1], "test.log");
    wathcer.watch();

}