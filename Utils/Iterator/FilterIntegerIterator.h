#pragma once
#include <cstddef>
#include <iterator>

class FilterIntegerIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = int;
    using pointer           = int*;
    using reference         = int&;
    using UnaryIntPredicate = bool(*)(int);

    FilterIntegerIterator(pointer ptr, size_t size, UnaryIntPredicate fun);

    reference operator*() const;
    pointer operator->();
    FilterIntegerIterator& operator++();
    FilterIntegerIterator operator++(int);

    friend bool operator==(const FilterIntegerIterator& a, const FilterIntegerIterator& b);
    friend bool operator!=(const FilterIntegerIterator& a, const FilterIntegerIterator& b);

    FilterIntegerIterator begin();
    FilterIntegerIterator end();

private:
    pointer m_end_ptr;
    pointer m_ptr;
    UnaryIntPredicate m_fun;
};