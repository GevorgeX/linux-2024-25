class ReverseArrayIterator{
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = int;
    using pointer           = int*;
    using reference         = int&;

    ReverseArrayIterator(pointer ptr, size_t size): m_ptr(ptr+size), m_end_ptr(ptr)
    {
    }

    reference operator*() const { return *(m_ptr); }
    pointer operator->() { return m_ptr; }
    ReverseArrayIterator& operator++()
    {
        m_ptr--;
        return *this;
    }
    ReverseArrayIterator operator++(int) { ReverseArrayIterator tmp = *this; ++(*this); return tmp; }
    friend bool operator== (const ReverseArrayIterator& a, const ReverseArrayIterator& b) { return a.m_ptr == b.m_ptr; };
    friend bool operator!= (const ReverseArrayIterator& a, const ReverseArrayIterator& b) { return a.m_ptr != b.m_ptr; };

    ReverseArrayIterator begin()
    {
        return ReverseArrayIterator(m_ptr, (m_end_ptr-m_ptr)>>sizeof (int));
    }
    ReverseArrayIterator end()
    {
        return ReverseArrayIterator(m_end_ptr,(m_end_ptr-m_ptr)>>sizeof (int) );
    }
private:
    pointer m_end_ptr;
    pointer m_ptr;
};