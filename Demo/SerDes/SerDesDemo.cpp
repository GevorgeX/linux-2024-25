#include <iostream>
#include <SerDes/SerDes.h>

struct  ST {
    int a;
    float b;
    char c;

    ST(){}
    ST(int a, float b, char c): a(a), b(b), c(c) {}
};


int main() {
    ST obj(19,-145.3, 'g');
    serialization("temp.txt",&obj);

    ST another_obj;
    deserialization("temp.txt",&another_obj);

    std::cout << another_obj.a << std::endl;
    std::cout << another_obj.b << std::endl;
    std::cout << another_obj.c << std::endl;
}