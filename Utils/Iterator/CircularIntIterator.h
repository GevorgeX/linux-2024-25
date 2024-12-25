#pragma once
#include <cstddef>
#include <iterator>

class CircularIntIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = int;
    using pointer           = int*;
    using reference         = int&;

    CircularIntIterator(pointer ptr, size_t size);

    reference operator*() const;
    pointer operator->();
    CircularIntIterator& operator++();
    CircularIntIterator operator++(int);

    friend bool operator==(const CircularIntIterator& a, const CircularIntIterator& b);
    friend bool operator!=(const CircularIntIterator& a, const CircularIntIterator& b);

    CircularIntIterator begin();
    CircularIntIterator end();

private:
    pointer m_end_ptr;
    size_t m_size;
    pointer m_ptr;

    CircularIntIterator(pointer ptr, pointer end, size_t size);
};