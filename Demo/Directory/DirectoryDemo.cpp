#include <iostream>
#include "Directory/Directory.h"

int main()
{
    Directory dir("TEST");
    for(auto t : dir)
    {
        std::cout<<t<<std::endl;
    }
}