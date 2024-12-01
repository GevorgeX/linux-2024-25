#include <iostream>
#include <DirectoryWatcher/DirectoryWatcher.h>

int main() {
    DirectoryWatcher wathcer("TEST");

    wathcer.watch();
}