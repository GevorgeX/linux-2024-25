#include <vector>
#include "Allocator/Allocator.h"

int main() {
    char buff[1000];
    Allocator<int> allocator(buff, 1000);
    std::vector<int, Allocator<int>> v(allocator);

    for (int i = 0; i < 40; i++) {
        v.push_back(i);
    }

    v.resize(20);
    v.shrink_to_fit();
    allocator.print();
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
}