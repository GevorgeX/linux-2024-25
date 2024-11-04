#include "ArgumentParser/ArgumentParser.h"

#include <iostream>
#include <ostream>

int main(int argc, char** argv)
{
    ArgumentParser s(argc, argv);

    s.parse("ab:cd");

    for (auto& t : s)
    {
        std::cout << t.flag<<" "<<*t.value << std::endl;
    }
}
