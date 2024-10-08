class FilterIntegerIterator{
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = int;
    using pointer           = int*;
    using reference         = int&;
    using UnraryIntPredicate = bool(*)(int);

    FilterIntegerIterator(pointer ptr, size_t size,UnraryIntPredicate fun): m_ptr(ptr), m_end_ptr(ptr+size),m_fun(fun)
    {
        while(!m_fun(*m_ptr) &&  m_ptr< m_end_ptr)
        {
            m_ptr++;
        }
    }

    reference operator*() const { return *m_ptr; }
    pointer operator->() { return m_ptr; }
    FilterIntegerIterator& operator++()
    {
        m_ptr++;
        while(!m_fun(*m_ptr) &&  m_ptr> m_end_ptr )
        {
            m_ptr++;
        }
        return *this;
    }
    FilterIntegerIterator operator++(int) { FilterIntegerIterator tmp = *this; ++(*this); return tmp; }
    friend bool operator== (const FilterIntegerIterator& a, const FilterIntegerIterator& b) { return a.m_ptr == b.m_ptr; };
    friend bool operator!= (const FilterIntegerIterator& a, const FilterIntegerIterator& b) { return a.m_ptr != b.m_ptr; };

    FilterIntegerIterator begin()
    {
        return FilterIntegerIterator(m_ptr, (m_end_ptr- m_ptr)>>sizeof (int), m_fun);
    }
    FilterIntegerIterator end()
    {
        return FilterIntegerIterator(m_end_ptr, (m_end_ptr- m_ptr)>>sizeof (int), m_fun);
    }
private:
    pointer m_end_ptr;
    pointer m_ptr;
    UnraryIntPredicate m_fun;
};