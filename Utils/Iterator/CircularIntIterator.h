class CircularIntIterator{
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = int;
    using pointer           = int*;
    using reference         = int&;

    CircularIntIterator(pointer ptr, size_t size): m_ptr(ptr), m_end_ptr(ptr+size),m_size(size)
    {
    }

    reference operator*() const { return *m_ptr; }
    pointer operator->() { return m_ptr; }
    CircularIntIterator& operator++()
    {
        m_ptr++;
        if(m_ptr == m_end_ptr)
            m_ptr -= m_size;

        return *this;
    }
    CircularIntIterator operator++(int) { CircularIntIterator tmp = *this; ++(*this); return tmp; }
    friend bool operator== (const  CircularIntIterator& a, const  CircularIntIterator& b) { return a.m_ptr == b.m_ptr; };
    friend bool operator!= (const  CircularIntIterator& a, const  CircularIntIterator& b) { return a.m_ptr != b.m_ptr; };

    CircularIntIterator begin()
    {
        return  CircularIntIterator(m_ptr, m_end_ptr, m_size);
    }
    CircularIntIterator end()
    {
        return  CircularIntIterator(m_end_ptr, m_end_ptr, m_size);
    }
private:
    pointer m_end_ptr;
    size_t m_size;
    pointer m_ptr;

    CircularIntIterator(pointer ptr,pointer end, size_t size): m_ptr(ptr), m_end_ptr(end),m_size(size)
    {
    }
};