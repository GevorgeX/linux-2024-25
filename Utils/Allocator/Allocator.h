#pragma once
#include <iostream>



template<class T>
class  Allocator
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

    Allocator(void* buffer, unsigned int size)
    {
        buff = new Block{buffer, size, true, nullptr};
    }

    T* allocate(std::size_t n)
    {
        auto size = n * sizeof(T);
        auto align = alignof(T);
        auto total_size = size;

        auto t = buff;
        while (t) {
            if (t->is_free && t->size >= total_size) {
                t->is_free = false;
                auto remaining = t->size - total_size;

                if(remaining > 0) {
                    t->size = total_size;
                    t->next = new Block{t->address+total_size, remaining,
                        true, t->next};
                }
                return static_cast<T*>(t->address);
            }
            t = t->next;
        }
        throw std::bad_alloc();
    }

    void deallocate(T* p, std::size_t) noexcept
    {

        auto t = buff;
        while (t) {
            if(t->address == p) {
                t->is_free = true;
                remove_free_spaces();
                return;
            }
            t = t->next;
        }
        throw std::invalid_argument("invalid pointer");
    }
    void remove_free_spaces() {
        auto t = buff;

        while (t->next) {
            if(t->is_free && t->next->is_free) {
                t->size += t->next->size;
                auto d = t->next;
                t->next = t->next->next;
                delete d;
            }
            else {
                t = t->next;
            }
        }
    }
    void print() {
        auto t = buff;
        while (t) {
            std::cout << "address: " << t->address << " size: " << t->size << " free: " << (t->is_free ? "yes" : "no") << "\n";
            t = t->next;
        }
    }
};

template<class T, class U>
bool operator==(const Allocator <T>&, const Allocator <U>&) { return true; }

template<class T, class U>
bool operator!=(const Allocator <T>&, const Allocator <U>&) { return false; }
