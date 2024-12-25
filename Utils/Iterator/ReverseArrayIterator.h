#pragma once
#include <cstddef>
#include <iterator>

class ReverseArrayIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = int;
    using pointer           = int*;
    using reference         = int&;

    ReverseArrayIterator(pointer ptr, size_t size);

    reference operator*() const;
    pointer operator->();
    ReverseArrayIterator& operator++();
    ReverseArrayIterator operator++(int);

    friend bool operator==(const ReverseArrayIterator& a, const ReverseArrayIterator& b);
    friend bool operator!=(const ReverseArrayIterator& a, const ReverseArrayIterator& b);

    ReverseArrayIterator begin();
    ReverseArrayIterator end();

private:
    pointer m_end_ptr;
    pointer m_ptr;
};