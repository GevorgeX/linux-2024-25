#include "CircularIntIterator.h"

CircularIntIterator::CircularIntIterator(pointer ptr, size_t size)
    : m_ptr(ptr), m_end_ptr(ptr + size), m_size(size) {}

CircularIntIterator::CircularIntIterator(pointer ptr, pointer end, size_t size)
    : m_ptr(ptr), m_end_ptr(end), m_size(size) {}

CircularIntIterator::reference CircularIntIterator::operator*() const {
    return *m_ptr;
}

CircularIntIterator::pointer CircularIntIterator::operator->() {
    return m_ptr;
}

CircularIntIterator& CircularIntIterator::operator++() {
    m_ptr++;
    if (m_ptr == m_end_ptr)
        m_ptr -= m_size;

    return *this;
}

CircularIntIterator CircularIntIterator::operator++(int) {
    CircularIntIterator tmp = *this;
    ++(*this);
    return tmp;
}

bool operator==(const CircularIntIterator& a, const CircularIntIterator& b) {
    return a.m_ptr == b.m_ptr;
}

bool operator!=(const CircularIntIterator& a, const CircularIntIterator& b) {
    return a.m_ptr != b.m_ptr;
}

CircularIntIterator CircularIntIterator::begin() {
    return CircularIntIterator(m_ptr, m_end_ptr, m_size);
}

CircularIntIterator CircularIntIterator::end() {
    return CircularIntIterator(m_end_ptr, m_end_ptr, m_size);
}
