#include "FilterIntegerIterator.h"

FilterIntegerIterator::FilterIntegerIterator(pointer ptr, size_t size, UnaryIntPredicate fun)
    : m_ptr(ptr), m_end_ptr(ptr + size), m_fun(fun) {
    while (!m_fun(*m_ptr) && m_ptr < m_end_ptr) {
        m_ptr++;
    }
}

FilterIntegerIterator::reference FilterIntegerIterator::operator*() const {
    return *m_ptr;
}

FilterIntegerIterator::pointer FilterIntegerIterator::operator->() {
    return m_ptr;
}

FilterIntegerIterator& FilterIntegerIterator::operator++() {
    m_ptr++;
    while (!m_fun(*m_ptr) && m_ptr < m_end_ptr) {
        m_ptr++;
    }
    return *this;
}

FilterIntegerIterator FilterIntegerIterator::operator++(int) {
    FilterIntegerIterator tmp = *this;
    ++(*this);
    return tmp;
}

bool operator==(const FilterIntegerIterator& a, const FilterIntegerIterator& b) {
    return a.m_ptr == b.m_ptr;
}

bool operator!=(const FilterIntegerIterator& a, const FilterIntegerIterator& b) {
    return a.m_ptr != b.m_ptr;
}

FilterIntegerIterator FilterIntegerIterator::begin() {
    return FilterIntegerIterator(m_ptr, (m_end_ptr - m_ptr) / sizeof(int), m_fun);
}

FilterIntegerIterator FilterIntegerIterator::end() {
    return FilterIntegerIterator(m_end_ptr, (m_end_ptr - m_ptr) / sizeof(int), m_fun);
}
