#pragma once
#include <iostream>

template<class T>
class Allocator
{
    struct Block {
        void* address;
        size_t size;
        bool is_free;
        Block* next;
    };

    Block* buff;
public:
    typedef T value_type;

    Allocator(void* buffer, unsigned int size);

    T* allocate(std::size_t n);
    void deallocate(T* p, std::size_t) noexcept;
    void remove_free_spaces();
    void print();
};

template<class T, class U>
bool operator==(const Allocator<T>&, const Allocator<U>&);

template<class T, class U>
bool operator!=(const Allocator<T>&, const Allocator<U>&);
