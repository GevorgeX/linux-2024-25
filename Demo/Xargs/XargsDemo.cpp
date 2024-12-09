#include <iostream>

#include "Xargs/Xargs.h"

int main(int argc, char* argv[]) {
    // test from console
    // like echo "/" | ./xargs ls -la
    xargs(argc, argv);
}