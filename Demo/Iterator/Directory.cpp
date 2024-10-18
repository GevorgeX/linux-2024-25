#include <iostream>
#include "Iterator/Directory.h"

int main()
{
    Directory dir("some path");
    for(auto& t : dir)
    {
        std::cout<<t.get_name()<<std::endl;
    }
}