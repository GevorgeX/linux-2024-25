#include "ReverseArrayIterator.h"

ReverseArrayIterator::ReverseArrayIterator(pointer ptr, size_t size)
    : m_ptr(ptr + size), m_end_ptr(ptr) {}

ReverseArrayIterator::reference ReverseArrayIterator::operator*() const {
    return *(m_ptr);
}

ReverseArrayIterator::pointer ReverseArrayIterator::operator->() {
    return m_ptr;
}

ReverseArrayIterator& ReverseArrayIterator::operator++() {
    m_ptr--;
    return *this;
}

ReverseArrayIterator ReverseArrayIterator::operator++(int) {
    ReverseArrayIterator tmp = *this;
    ++(*this);
    return tmp;
}

bool operator==(const ReverseArrayIterator& a, const ReverseArrayIterator& b) {
    return a.m_ptr == b.m_ptr;
}

bool operator!=(const ReverseArrayIterator& a, const ReverseArrayIterator& b) {
    return a.m_ptr != b.m_ptr;
}

ReverseArrayIterator ReverseArrayIterator::begin() {
    return ReverseArrayIterator(m_ptr, (m_ptr - m_end_ptr) / sizeof(int));
}

ReverseArrayIterator ReverseArrayIterator::end() {
    return ReverseArrayIterator(m_end_ptr, (m_ptr - m_end_ptr) / sizeof(int));
}
