#include <iostream>
#include <SerDes/SerDes.h>

struct ST {
    unsigned int a;
    long long b;
    char c;

    ST(){}
    ST(unsigned int a, long long b, char c): a(a), b(b), c(c) {}
};

int main() {
    std::cout << "TEST 1\n";
    {
        ST obj(30,-30, 'g');
        serialization("temp.txt",obj);

        ST another_obj;
        deserialization("temp.txt",another_obj);

        std::cout<<"First field: " << obj.a <<" == "<< another_obj.a << std::endl;
        std::cout<<"Second field: "<< obj.b <<" == "<< another_obj.b << std::endl;
        std::cout<<"Third field: " << obj.c <<" == "<< another_obj.c << std::endl;
    }

    std::cout << "TEST 2\n";
    {
        ST obj(300,12, 'A');
        serialization("temp2.txt",obj);

        ST another_obj;
        deserialization("temp2.txt",another_obj);

        std::cout<<"First field: " << obj.a <<" == "<< another_obj.a << std::endl;
        std::cout<<"Second field: "<< obj.b <<" == "<< another_obj.b << std::endl;
        std::cout<<"Third field: " << obj.c <<" == "<< another_obj.c << std::endl;
    }

    std::cout << "TEST 3\n";
    {
        ST obj(1523,1541235135, 54);
        serialization("temp3.txt",obj);

        ST another_obj;
        deserialization("temp3.txt",another_obj);

        std::cout<<"First field: " << obj.a <<" == "<< another_obj.a << std::endl;
        std::cout<<"Second field: "<< obj.b <<" == "<< another_obj.b << std::endl;
        std::cout<<"Third field: " << obj.c <<" == "<< another_obj.c << std::endl;
    }
}