#ifndef DEQUE_HPP
#define DEQUE_HPP

#include "strings.defs.h"
#include "iterator.hpp"
#include "memory.hpp"
#include "algorithm.hpp"

namespace saber
{

template <typename T, typename Allocator = default_allocator<T>>
class deque
{
    static_assert(std::is_copy_constructible<T>::value,
                  C8_STAT__TEMPLATE_ARG__T__COPY_CONSTRUCT_ERROR);
    static_assert(std::is_destructible<T>::value,
                  C8_STAT__TEMPLATE_ARG__T__DESTROY_ERROR);

public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using allocator_type = Allocator;

    class iterator;
    class const_iterator;

    using reverse_iterator = saber::reverse_iterator<iterator>;
    using const_reverse_iterator = saber::reverse_iterator<const_iterator>;

    deque();
    explicit deque(const Allocator& _alloc);
    explicit deque(size_type _n,
                   const Allocator& _alloc = Allocator());
    deque(size_type _n, const T& _value,
          const Allocator& _alloc = Allocator());
    deque(const deque& _another);
    deque(deque&& _another);
    deque(const deque& _another, const Allocator& _alloc);
    deque(deque&& _another, const Allocator& _alloc);
    deque(initializer_list<T> _ilist,
          const Allocator& _alloc = Allocator());
    template <typename InputIterator>
    deque(InputIterator _first, InputIterator _last,
          const Allocator& _alloc = Allocator());
    ~deque();

    deque& operator= (const deque& _another);
    deque& operator= (deque&& _another);
    deque& operator= (initializer_list<T> _ilist);
    
    template <typename InputIterator>
    void assign(InputIterator _first, InputIterator _last);
    void assign(size_type _n, const value_type& _value);
    void assign(initializer_list<T> _ilist);

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    reverse_iterator rbegin() noexcept;
    reverse_iterator rend() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;

    size_type size() const noexcept;
    size_type max_size() const noexcept;
    bool empty() const noexcept;

    reference at(size_type _n);
    const_reference at(size_type _n) const;

    reference operator[](size_type _n);
    const_reference operator[](size_type _n) const;

    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    template <typename... Args>
    void emplace_front(Args&&... _args);
    template <typename... Args>
    void emplace_back(Args&&... _args);
    template <typename... Args>
    void emplace(const_iterator _pos, Args&&... _args);

    void push_back(const value_type& _value);
    void push_front(const value_type& _value);
    void push_back(value_type&& _value);
    void push_front(value_type&& _value);
    void pop_front();
    void pop_back();

    iterator insert(const_iterator _pos, const value_type& _value);
    // iterator insert(const_iterator _pos, value_type&& _value);
    iterator insert(const_iterator _pos,
                    size_type _n, const value_type& _value);
    template <typename InputIterator>
    iterator insert(const_iterator _pos,
                    InputIterator _first, InputIterator _last);
    iterator insert(const_iterator _pos, initializer_list<T> _ilist);

    iterator erase(const_iterator _position);
    iterator erase(const_iterator _first, const_iterator _last);

    void clear();
    void swap(deque& _another);

private:
    using subarray_type = T*;
    using ptr_allocator_type =
        typename Allocator:: template rebind<subarray_type>::other;

    enum {subarray_size = 16};

    struct cmap
    {
        subarray_type *ptr_array;
        size_type array_count;
    }
    *center_map;

    class cmap_iterator;

    void add_subarray_at_begin();
    void add_subarray_at_end();

    subarray_type create_subarray();
    void update_vector();

    cmap_iterator *cmap_it_begin = nullptr;
    cmap_iterator *cmap_it_end = nullptr;
    cmap_iterator *cmap_it_cap_begin = nullptr;
    cmap_iterator *cmap_it_cap_end = nullptr;

    allocator_type alloc;
    ptr_allocator_type ptr_alloc;

    saber_ptr<bool> validating_ptr;
    uint32_t update_count = 0;
};

//////////////////////////////////////////////////////////////////////////////
// class iterator                                                           //
//////////////////////////////////////////////////////////////////////////////

template <typename T, typename Allocator>
class deque<T, Allocator>::iterator
{
    friend class deque;
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = typename deque::value_type;
    using difference_type   = typename deque::difference_type;
    using pointer           = typename deque::pointer;
    using reference         = typename deque::reference;
    using const_reference   = typename deque::const_reference;

    iterator(void) = default;
    iterator(const iterator&) = default;
    // See C++ Defect Report #179
    iterator(const typename deque::const_iterator& _const_iterator);
    ~iterator() = default;

    reference operator* (void);
    const_reference operator* (void) const;

    reference operator[] (difference_type _diff);
    const_reference operator[] (difference_type _diff) const;

    bool operator== (const iterator& _another) const;
    bool operator!= (const iterator& _another) const;

    bool operator< (const iterator& _another) const;
    bool operator> (const iterator& _another) const;
    bool operator<= (const iterator& _another) const;
    bool operator>= (const iterator& _another) const;

    iterator& operator++ (void);
    iterator& operator-- (void);
    iterator operator++ (int);
    iterator operator-- (int);

    iterator& operator+= (difference_type _diff);
    iterator& operator-= (difference_type _diff);
    iterator operator+ (difference_type _diff);
    iterator operator- (difference_type _diff);

    difference_type operator- (const iterator& _another);

private:
    using cmap_iterator = typename deque::cmap_iterator;
    const deque *get_from = nullptr;
    cmap_iterator cmap_it;
    uint32_t update_count = 0;
    saber_ptr<bool> validating_ptr;

    void check_initialized() const;
    void check_up_to_date() const;
    void check_dereferencable() const;

    iterator(const deque *_get_from,
             const cmap_iterator& cmap_it) :
        get_from(_get_from),
        cmap_it(cmap_it),
        update_count(get_from->update_count),
        validating_ptr(get_from->validating_ptr)
    {}
};

//////////////////////////////////////////////////////////////////////////////
// class const_iterator                                                     //
//////////////////////////////////////////////////////////////////////////////

template <typename T, typename Allocator>
class deque<T, Allocator>::const_iterator
{
    friend class deque;
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = typename deque::value_type;
    using difference_type = typename deque::difference_type;
    using pointer = typename deque::pointer;
    using reference = typename deque::reference;
    using const_reference = typename deque::const_reference;

    const_iterator(void) = default;
    const_iterator(const const_iterator&) = default;
    // See C++ Defect Report #179
    const_iterator(const typename deque::iterator& _mutable_iterator);
    ~const_iterator() = default;

    const_reference operator* (void) const;
    const_reference operator[] (difference_type _diff) const;

    bool operator== (const const_iterator& _another) const;
    bool operator!= (const const_iterator& _another) const;

    bool operator< (const const_iterator& _another) const;
    bool operator> (const const_iterator& _another) const;
    bool operator<= (const const_iterator& _another) const;
    bool operator>= (const const_iterator& _another) const;

    const_iterator& operator+= (difference_type _diff);
    const_iterator& operator-= (difference_type _diff);
    const_iterator operator+ (difference_type _diff);
    const_iterator operator- (difference_type _diff);

    const_iterator& operator++ (void);
    const_iterator& operator-- (void);
    const_iterator operator++ (int);
    const_iterator operator-- (int);

    difference_type operator- (const const_iterator& _another);

private:
    using cmap_iterator = typename deque::cmap_iterator;
    const deque *get_from = nullptr;
    cmap_iterator cmap_it;
    uint32_t update_count = 0;
    saber_ptr<bool> validating_ptr;

    void check_initialized() const;
    void check_up_to_date() const;
    void check_dereferencable() const;

    const_iterator(const deque *_get_from,
                   const cmap_iterator& cmap_it) :
        get_from(_get_from),
        cmap_it(cmap_it),
        update_count(get_from->update_count),
        validating_ptr(get_from->validating_ptr)
    {}
};

//////////////////////////////////////////////////////////////////////////////
// class cmap_iterator                                                      //
//////////////////////////////////////////////////////////////////////////////

template <typename T, typename Allocator>
class deque<T, Allocator>::cmap_iterator
{
    friend class deque;
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = typename deque::value_type;
    using difference_type   = typename deque::difference_type;
    using pointer           = typename deque::pointer;
    using reference         = typename deque::reference;
    using const_reference   = typename deque::const_reference;

    cmap_iterator(void) = default;
    cmap_iterator(const cmap_iterator&) = default;
    ~cmap_iterator() = default;

    reference operator* (void);
    const_reference operator* (void) const;

    bool operator== (const cmap_iterator& _another) const;
    bool operator!= (const cmap_iterator& _another) const;
    bool operator< (const cmap_iterator& _another) const;

    cmap_iterator& operator++(void);
    cmap_iterator& operator--(void);

    cmap_iterator& operator+= (difference_type _diff);
    difference_type operator- (const cmap_iterator& _another);

private:
    using size_type     = typename deque::size_type;
    using subarray_type = typename deque::subarray_type;
    using cmap          = typename deque::cmap;

    cmap_iterator(const cmap* _p_cmap,
                  size_type _subarray_ptr,
                  size_type _subarray_index) :
        p_cmap(_p_cmap),
        subarray_ptr(_subarray_ptr),
        index(_subarray_index)
    {}

    const cmap *p_cmap = nullptr;
    size_type subarray_ptr = 0;
    difference_type index = 0;
};

//////////////////////////////////////////////////////////////////////////////
// Implementations of functions                                             //
/// //////////////////////////////////////////////////////////////////////////

template <typename T, typename Allocator>
deque<T, Allocator>::deque(const Allocator& _alloc) :
    alloc(_alloc),
    ptr_alloc(alloc),
    validating_ptr(new bool(true))
{
    center_map = new cmap;

    center_map->ptr_array =
            allocator_traits<ptr_allocator_type>::allocate(ptr_alloc, 2);
    center_map->ptr_array[0] = create_subarray();
    center_map->ptr_array[1] = create_subarray();
    center_map->array_count = 2;

    cmap_it_cap_begin = new cmap_iterator(center_map, 0, 0);
    cmap_it_cap_end = new cmap_iterator(center_map, 1, subarray_size-1);
    cmap_it_begin = new cmap_iterator(center_map, 1, 0);
    cmap_it_end = new cmap_iterator(center_map, 1, 0);
}

template <typename T, typename Allocator>
deque<T, Allocator>::deque() :
    deque(Allocator())
{
}

template <typename T, typename Allocator>
deque<T, Allocator>::deque(size_type _n, const Allocator& _alloc) :
    deque(_n, value_type(), _alloc)
{
}

template <typename T, typename Allocator>
deque<T, Allocator>::deque(size_type _n, const T &_value,
                           const Allocator &_alloc) :
    deque(_alloc)
{
    for (size_type i = 0; i < _n; i++)
    {
        push_back(_value);
    }
}

template <typename T, typename Allocator>
deque<T, Allocator>::deque(const deque &_another) :
    deque(_another, _another.alloc)
{
}

template <typename T, typename Allocator>
deque<T, Allocator>::deque(const deque &_another, const Allocator &_alloc) :
    deque(_another.cbegin(), _another.cend(), _alloc)
{
    stl_warning(C8_DYN__CONT__CONTAINER_COPY);
}

template <typename T, typename Allocator>
deque<T, Allocator>::deque(deque &&_another) :
    deque(std::move(_another), _another.alloc)

{
}

template <typename T, typename Allocator>
deque<T, Allocator>::deque(deque &&_another, const Allocator &_alloc) :
    deque(_alloc)
{
    swap(_another);
}

template <typename T, typename Allocator>
deque<T, Allocator>::deque(initializer_list<T> _ilist,
                           const Allocator &_alloc) :
    deque(_ilist.begin(), _ilist.end(), _alloc)
{
}

template <typename T, typename Allocator>
template <typename InputIterator>
deque<T, Allocator>::deque(InputIterator _first, InputIterator _last,
                           const Allocator& _alloc) :
    deque(_alloc)
{
    for (; _first != _last; ++_first)
    {
        push_back(*_first);
    }
}

template <typename T, typename Allocator>
deque<T, Allocator>::~deque()
{
    clear();
    for (size_type i = 0; i < center_map->array_count; ++i)
    {
        allocator_traits<allocator_type>::deallocate(
                    alloc, center_map->ptr_array[i], subarray_size);
    }
    allocator_traits<ptr_allocator_type>::deallocate(
                ptr_alloc, center_map->ptr_array, center_map->array_count);

    delete cmap_it_begin;
    delete cmap_it_end;
    delete cmap_it_cap_begin;
    delete cmap_it_cap_end;
    (*validating_ptr.get()) = false;

    delete center_map;
}

template <typename T, typename Allocator>
deque<T, Allocator>&
deque<T, Allocator>::operator= (const deque& _another)
{
    deque temp(_another);
    swap(temp);
    return *this;
}

template <typename T, typename Allocator>
deque<T, Allocator>&
deque<T, Allocator>::operator= (deque&& _another)
{
    deque temp(_another);
    swap(temp);
    return *this;
}

template <typename T, typename Allocator>
deque<T, Allocator>&
deque<T, Allocator>::operator= (initializer_list<T> _ilist)
{
    deque temp(_ilist);
    swap(temp);
    return *this;
}

template <typename T, typename Allocator>
template <typename InputIterator>
void
deque<T, Allocator>::assign(InputIterator _first, InputIterator _last)
{
    deque temp(_first, _last);
    swap(temp);
}

template <typename T, typename Allocator>
void
deque<T, Allocator>::assign(size_type _n, const value_type &_value)
{
    deque temp(_n, _value);
    swap(temp);
}

template <typename T, typename Allocator>
void
deque<T, Allocator>::assign(initializer_list<T> _ilist)
{
    this->operator= (_ilist);
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator
deque<T, Allocator>::begin() noexcept
{
    return iterator(this, *cmap_it_begin);
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator
deque<T, Allocator>::end() noexcept
{
    return iterator(this, *cmap_it_end);
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_iterator
deque<T, Allocator>::begin() const noexcept
{
    return cbegin();
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_iterator
deque<T, Allocator>::end() const noexcept
{
    return cend();
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_iterator
deque<T, Allocator>::cbegin() const noexcept
{
    return const_iterator(this, *cmap_it_begin);
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_iterator
deque<T, Allocator>::cend() const noexcept
{
    return const_iterator(this, *cmap_it_end);
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::reverse_iterator
deque<T, Allocator>::rbegin() noexcept
{
    return reverse_iterator(end());
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::reverse_iterator
deque<T, Allocator>::rend() noexcept
{
    return reverse_iterator(begin());
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_reverse_iterator
deque<T, Allocator>::rbegin() const noexcept
{
    return crbegin();
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_reverse_iterator
deque<T, Allocator>::rend() const noexcept
{
    return crend();
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_reverse_iterator
deque<T, Allocator>::crbegin() const noexcept
{
    return reverse_iterator(end());
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_reverse_iterator
deque<T, Allocator>::crend() const noexcept
{
    return reverse_iterator(begin());
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::size_type
deque<T, Allocator>::size() const noexcept
{
    return (cend() - cbegin());
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::size_type
deque<T, Allocator>::max_size() const noexcept
{
    return std::numeric_limits<size_type>::max();
}

template <typename T, typename Allocator>
bool
deque<T, Allocator>::empty() const noexcept
{
    return (cend() == cbegin());
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::reference
deque<T, Allocator>::at(size_type _n)
{
    if (_n >= size())
        throw std::out_of_range(C8_DYN__ITER__SUBSCRIPT_OVERFLOW);
    return *(begin() + _n);
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_reference
deque<T, Allocator>::at(size_type _n) const
{
    if (_n >= size())
        throw std::out_of_range(C8_DYN__ITER__SUBSCRIPT_OVERFLOW);
    return *(cbegin() + _n);
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::reference
deque<T, Allocator>::operator[] (size_type _n)
{
    if (_n >= size()) stl_panic(C8_DYN__ITER__SUBSCRIPT_OVERFLOW);
    return *(begin() + _n);
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_reference
deque<T, Allocator>::operator[] (size_type _n) const
{
    if (_n >= size()) stl_panic(C8_DYN__ITER__SUBSCRIPT_OVERFLOW);
    return *(cbegin() + _n);
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::reference
deque<T, Allocator>::front()
{
    return *begin();
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::reference
deque<T, Allocator>::back()
{
    return *(end()-1);
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_reference
deque<T, Allocator>::front() const
{
    return *cbegin();
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_reference
deque<T, Allocator>::back() const
{
    return *(cend()-1);
}

template <typename T, typename Allocator>
template <typename... Args>
void
deque<T, Allocator>::emplace_back(Args&&... _args)
{
    if (*cmap_it_end == *cmap_it_cap_end) add_subarray_at_end();
    construct(std::addressof(cmap_it_end->operator*()),
              saber::forward<Args>(_args)...);
    cmap_it_end->operator++();

    update_vector();
}

template <typename T, typename Allocator>
template <typename... Args>
void
deque<T, Allocator>::emplace_front(Args&&... _args)
{
    if (*cmap_it_begin == *cmap_it_cap_begin) add_subarray_at_begin();
    cmap_iterator by_the_way = *cmap_it_begin;
    --by_the_way;
    construct(std::addressof(*by_the_way),
              saber::forward<Args>(_args)...);
    cmap_it_begin->operator--();

    update_vector();
}

template <typename T, typename Allocator>
template <typename... Args>
void
deque<T, Allocator>::emplace(const_iterator _pos, Args&&... _args)
{
    difference_type _pos_diff = _pos - cbegin();

    if (_pos == cend())
    {
        emplace_back(saber::forward<Args>(_args)...);
    }
    else if (_pos == cbegin())
    {
        emplace_front(saber::forward<Args>(_args)...);
    }
    else
    {
        if (_pos - cbegin() < cend() - _pos)
        {
            emplace_front(front());
            _pos = const_iterator(cbegin() + _pos_diff + 1);
            copy(cbegin()+1, _pos, begin());
            --_pos;
        }
        else
        {
            emplace_back(back());
            _pos = const_iterator(cbegin() + _pos_diff);
            reverse_copy(_pos, --cend(), rbegin());
        }

        destroy_at(std::addressof(*iterator(_pos)));
        construct(std::addressof(*iterator(_pos)),
                  saber::forward<Args>(_args)...);
    }
}

template <typename T, typename Allocator>
void
deque<T, Allocator>::push_back(const value_type &_value)
{
    emplace_back(_value);
}

template <typename T, typename Allocator>
void
deque<T, Allocator>::push_front(const value_type &_value)
{
    emplace_front(_value);
}

template <typename T, typename Allocator>
void
deque<T, Allocator>::push_back(value_type &&_value)
{
    emplace_back(std::move(_value));
}

template <typename T, typename Allocator>
void
deque<T, Allocator>::push_front(value_type &&_value)
{
    emplace_front(std::move(_value));
}

template <typename T, typename Allocator>
void
deque<T, Allocator>::pop_back()
{
    destroy_at(std::addressof(*end()));
    cmap_it_end--;
    update_vector();
}

template <typename T, typename Allocator>
void
deque<T, Allocator>::pop_front()
{
    destroy_at(std::addressof(*begin()));
    cmap_it_begin++;
    update_vector();
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator
deque<T, Allocator>::insert(const_iterator _pos, const value_type &_value)
{
    static_assert(std::is_copy_constructible<T>::value,
                  C8_STAT__TEMPLATE_ARG__T__COPY_CONSTRUCT_ERROR);

    difference_type pos_diff = _pos - cbegin();
    emplace(_pos, _value);
    update_vector();
    return iterator(begin() + pos_diff);
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator
deque<T, Allocator>::insert(const_iterator _pos,
                            size_type _n,
                            const value_type &_value)
{
    // This is just a *temporary* solution
    // implement this insert by inserting elements one by one.

    static_assert(std::is_copy_assignable<T>::value,
                  C8_STAT__TEMPLATE_ARG__T__COPY_ASSIGN_ERROR);

    for (size_type i = 0; i < _n; ++i)
    {
        _pos = insert(_pos, _value);
        ++_pos;
    }
    return _pos;
}

template <typename T, typename Allocator>
template <typename InputIterator>
typename deque<T, Allocator>::iterator
deque<T, Allocator>::insert(const_iterator _pos,
                            InputIterator _first, InputIterator _last)
{
    static_assert(std::is_copy_assignable<T>::value,
                  C8_STAT__TEMPLATE_ARG__T__COPY_ASSIGN_ERROR);

    for (; _first != _last; ++_first)
    {
        _pos = insert(_pos, *_first);
        ++_pos;
    }
    return _pos;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator
deque<T, Allocator>::insert(const_iterator _pos, initializer_list<T> _ilist)
{
    static_assert(std::is_copy_assignable<T>::value,
                  C8_STAT__TEMPLATE_ARG__T__COPY_ASSIGN_ERROR);

    return insert(_pos, _ilist.begin(), _ilist.end());
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator
deque<T, Allocator>::erase(const_iterator _position)
{
    static_assert(std::is_copy_assignable<T>::value,
                  C8_STAT__TEMPLATE_ARG__T__COPY_ASSIGN_ERROR);

    return erase(_position, _position+1);
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator
deque<T, Allocator>::erase(const_iterator _first, const_iterator _last)
{
    if ( (_first - cbegin()) < (_last - cend()) )
    {
        iterator begin_of_storage =
                reverse_copy(cbegin(), _first, iterator(_last));
        destroy(begin(), begin_of_storage);
        cmap_it_begin->operator= (begin_of_storage.cmap_it);
        update_vector();
        return iterator(this, begin_of_storage.cmap_it);
    }
    else
    {
        iterator end_of_storage =
                copy(_last, cend(), iterator(_first));
        destroy(end_of_storage, end());
        cmap_it_end->operator= (end_of_storage.cmap_it);
        update_vector();
        return iterator(this, end_of_storage.cmap_it);
    }
}

template <typename T, typename Allocator>
void
deque<T, Allocator>::clear()
{
    erase(cbegin(), cend());
}

template <typename T, typename Allocator>
void
deque<T, Allocator>::swap(deque &_another)
{
    saber::swap(alloc, _another.alloc);
    saber::swap(ptr_alloc, _another.ptr_alloc);
    saber::swap(center_map, _another.center_map);
    saber::swap(cmap_it_begin, _another.cmap_it_begin);
    saber::swap(cmap_it_end, _another.cmap_it_end);
    saber::swap(cmap_it_cap_begin, _another.cmap_it_cap_begin);
    saber::swap(cmap_it_cap_end, _another.cmap_it_cap_end);
    saber::swap(validating_ptr, _another.validating_ptr);

    update_vector();
    _another.update_vector();
}

template <typename T, typename Allocator>
void
deque<T, Allocator>::add_subarray_at_begin()
{
    subarray_type *new_ptr_array =
            allocator_traits<ptr_allocator_type>::allocate(
                ptr_alloc, center_map->array_count+1);
    new_ptr_array[0] = create_subarray();
    uninitialized_copy_n(center_map->ptr_array, center_map->array_count,
                         new_ptr_array+1);

    allocator_traits<ptr_allocator_type>::deallocate(ptr_alloc,
                                                     center_map->ptr_array,
                                                     center_map->array_count);
    center_map->ptr_array = new_ptr_array;
    center_map->array_count++;

    cmap_it_begin->subarray_ptr++;
    cmap_it_end->subarray_ptr++;
    cmap_it_cap_end->subarray_ptr++;
}

template <typename T, typename Allocator>
void
deque<T, Allocator>::add_subarray_at_end()
{
    subarray_type *new_ptr_array =
            allocator_traits<ptr_allocator_type>::allocate(
                ptr_alloc, center_map->array_count+1);
    new_ptr_array[center_map->array_count] = create_subarray();
    uninitialized_copy_n(center_map->ptr_array, center_map->array_count,
                         new_ptr_array);

    allocator_traits<ptr_allocator_type>::deallocate(ptr_alloc,
                                                     center_map->ptr_array,
                                                     center_map->array_count);
    center_map->ptr_array = new_ptr_array;
    center_map->array_count++;

    cmap_it_cap_end->subarray_ptr++;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::subarray_type
deque<T, Allocator>::create_subarray()
{
    subarray_type subarray =
            allocator_traits<allocator_type>::allocate(alloc, subarray_size);
    return subarray;
}

template <typename T, typename Allocator>
void
deque<T, Allocator>::update_vector()
{
    update_count++;
    if (update_count == std::numeric_limits<decltype(update_count)>::max())
    {
        update_count = 0;
    }
}



template <typename T, typename Allocator>
deque<T, Allocator>::iterator::iterator(
        const typename deque::const_iterator& _const_iterator) :
    iterator(_const_iterator.get_from,
             _const_iterator.cmap_it)
{
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator::reference
deque<T, Allocator>::iterator::operator* ()
{
    check_dereferencable();
    return cmap_it.operator* ();
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator::const_reference
deque<T, Allocator>::iterator::operator* () const
{
    check_dereferencable();
    return cmap_it.operator* ();
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator::reference
deque<T, Allocator>::iterator::operator[] (difference_type _diff)
{
    check_dereferencable();
    return *(*this + _diff);
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator::const_reference
deque<T, Allocator>::iterator::operator[] (difference_type _diff) const
{
    check_dereferencable();
    return *(*this + _diff);
}

template <typename T, typename Allocator>
bool
deque<T, Allocator>::iterator::operator== (const iterator& _another) const
{
    check_up_to_date();
    return cmap_it == _another.cmap_it;
}

template <typename T, typename Allocator>
bool
deque<T, Allocator>::iterator::operator!= (const iterator& _another) const
{
    check_up_to_date();
    return cmap_it != _another.cmap_it;
}

template <typename T, typename Allocator>
bool
deque<T, Allocator>::iterator::operator< (const iterator& _another) const
{
    check_up_to_date();
    return cmap_it < _another.cmap_it;
}

template <typename T, typename Allocator>
bool
deque<T, Allocator>::iterator::operator> (const iterator& _another) const
{
    return !operator< (_another);
}

template <typename T, typename Allocator>
bool
deque<T, Allocator>::iterator::operator<= (const iterator& _another) const
{
    return !operator>(_another);
}

template <typename T, typename Allocator>
bool
deque<T, Allocator>::iterator::operator>= (const iterator& _another) const
{
    return !operator<(_another);
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator&
deque<T, Allocator>::iterator::operator++ ()
{
    check_up_to_date();
    ++cmap_it;
    return *this;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator&
deque<T, Allocator>::iterator::operator-- ()
{
    check_up_to_date();
    --cmap_it;
    return *this;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator
deque<T, Allocator>::iterator::operator++ (int)
{
    iterator temp = *this;
    operator++ ();
    return temp;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator
deque<T, Allocator>::iterator::operator-- (int)
{
    iterator temp = *this;
    operator-- ();
    return temp;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator&
deque<T, Allocator>::iterator::operator+= (difference_type _diff)
{
    check_up_to_date();
    cmap_it += _diff;
    return *this;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator&
deque<T, Allocator>::iterator::operator-= (difference_type _diff)
{
    check_up_to_date();
    cmap_it += -1* _diff;
    return *this;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator
deque<T, Allocator>::iterator::operator+ (difference_type _diff)
{
    iterator ret = *this;
    ret += _diff;
    return ret;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator
deque<T, Allocator>::iterator::operator- (difference_type _diff)
{
    iterator ret = *this;
    ret -= _diff;
    return ret;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::iterator::difference_type
deque<T, Allocator>::iterator::operator- (const iterator& _another)
{
    check_up_to_date();
    _another.check_up_to_date();
    return cmap_it - _another.cmap_it;
}

template <typename T, typename Allocator>
void
deque<T, Allocator>::iterator::check_initialized() const
{
    if (get_from == nullptr)
    {
        assert(update_count == 0);
        stl_panic(C8_DYN__ITER__UNINITIALIZED_ITERATOR);
    }

    if (*(validating_ptr.get()) == false)
    {
        stl_panic(C8_DYN__ITER__DELETED_CONTAINER);
    }
}

template <typename T, typename Allocator>
void
deque<T, Allocator>::iterator::check_up_to_date() const
{
    check_initialized();
    if (update_count != get_from->update_count)
    {
        stl_panic(C8_DYN__ITER__OLD_ITERATOR);
    }
}

template <typename T, typename Allocator>
void
deque<T, Allocator>::iterator::check_dereferencable() const
{
    check_up_to_date();
    if (update_count != get_from->update_count)
    {
        stl_panic(C8_DYN__ITER__OLD_ITERATOR);
    }

    if (! (*this < get_from->end()
           && *this >= get_from->begin()))
    {
        stl_panic(C8_DYN__ITER__ITERATOR_OVERFLOW);
    }
}



template <typename T, typename Allocator>
deque<T, Allocator>::const_iterator::const_iterator(
        const typename deque::iterator& _mutable_iterator) :
    const_iterator(_mutable_iterator.get_from,
                   _mutable_iterator.cmap_it)
{
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_iterator::const_reference
deque<T, Allocator>::const_iterator::operator* () const
{
    check_dereferencable();
    return cmap_it.operator* ();
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_iterator::const_reference
deque<T, Allocator>::const_iterator::operator[] (difference_type _diff) const
{
    check_dereferencable();
    return *(*this + _diff);
}

template <typename T, typename Allocator>
bool
deque<T, Allocator>::const_iterator::operator== (
        const const_iterator& _another) const
{
    check_up_to_date();
    return cmap_it == _another.cmap_it;
}

template <typename T, typename Allocator>
bool
deque<T, Allocator>::const_iterator::operator!= (
        const const_iterator& _another) const
{
    check_up_to_date();
    return cmap_it != _another.cmap_it;
}

template <typename T, typename Allocator>
bool
deque<T, Allocator>::const_iterator::operator< (
        const const_iterator& _another) const
{
    check_up_to_date();
    return cmap_it < _another.cmap_it;
}

template <typename T, typename Allocator>
bool
deque<T, Allocator>::const_iterator::operator> (
        const const_iterator& _another) const
{
    return !operator< (_another);
}

template <typename T, typename Allocator>
bool
deque<T, Allocator>::const_iterator::operator<= (
        const const_iterator& _another) const
{
    return !operator> (_another);
}

template <typename T, typename Allocator>
bool
deque<T, Allocator>::const_iterator::operator>= (
        const const_iterator& _another) const
{
    return !operator< (_another);
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_iterator&
deque<T, Allocator>::const_iterator::operator++ ()
{
    check_up_to_date();
    ++cmap_it;
    return *this;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_iterator&
deque<T, Allocator>::const_iterator::operator-- ()
{
    check_up_to_date();
    --cmap_it;
    return *this;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_iterator
deque<T, Allocator>::const_iterator::operator++ (int)
{
    const_iterator temp = *this;
    operator++ ();
    return temp;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_iterator
deque<T, Allocator>::const_iterator::operator-- (int)
{
    const_iterator temp = *this;
    operator-- ();
    return temp;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_iterator&
deque<T, Allocator>::const_iterator::operator+= (difference_type _diff)
{
    check_up_to_date();
    cmap_it += _diff;
    return *this;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_iterator&
deque<T, Allocator>::const_iterator::operator-= (difference_type _diff)
{
    check_up_to_date();
    cmap_it += (-1 * _diff);
    return *this;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_iterator
deque<T, Allocator>::const_iterator::operator+ (difference_type _diff)
{
    const_iterator ret = *this;
    ret += _diff;
    return ret;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_iterator
deque<T, Allocator>::const_iterator::operator- (difference_type _diff)
{
    const_iterator ret = *this;
    ret -= _diff;
    return ret;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::const_iterator::difference_type
deque<T, Allocator>::const_iterator::operator- (const const_iterator& _another)
{
    check_up_to_date();
    _another.check_up_to_date();
    return cmap_it - _another.cmap_it;
}

template <typename T, typename Allocator>
void
deque<T, Allocator>::const_iterator::check_initialized() const
{
    if (get_from == nullptr)
    {
        assert(update_count == 0);
        stl_panic(C8_DYN__ITER__UNINITIALIZED_ITERATOR);
    }

    if (*(validating_ptr.get()) == false)
    {
        stl_panic(C8_DYN__ITER__DELETED_CONTAINER);
    }
}

template <typename T, typename Allocator>
void
deque<T, Allocator>::const_iterator::check_up_to_date() const
{
    check_initialized();
    if (update_count != get_from->update_count)
    {
        stl_panic(C8_DYN__ITER__OLD_ITERATOR);
    }
}

template <typename T, typename Allocator>
void
deque<T, Allocator>::const_iterator::check_dereferencable() const
{
    check_up_to_date();
    if (update_count != get_from->update_count)
    {
        stl_panic(C8_DYN__ITER__OLD_ITERATOR);
    }

    if (! (*this < get_from->end()
           && *this >= get_from->begin()))
    {
        stl_panic(C8_DYN__ITER__ITERATOR_OVERFLOW);
    }
}



template <typename T, typename Allocator>
typename deque<T, Allocator>::cmap_iterator::reference
deque<T, Allocator>::cmap_iterator::operator* ()
{
    return p_cmap->ptr_array[subarray_ptr][index];
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::cmap_iterator::const_reference
deque<T, Allocator>::cmap_iterator::operator* () const
{
    return p_cmap->ptr_array[subarray_ptr][index];
}

template <typename T, typename Allocator>
bool
deque<T, Allocator>::cmap_iterator::operator== (
        const cmap_iterator& _another) const
{
    return (p_cmap == _another.p_cmap)
           && (subarray_ptr == _another.subarray_ptr)
           && (index == _another.index);
}

template <typename T, typename Allocator>
bool
deque<T, Allocator>::cmap_iterator::operator!= (
        const cmap_iterator& _another) const
{
    return !operator== (_another);
}

template <typename T, typename Allocator>
bool
deque<T, Allocator>::cmap_iterator::operator< (
        const cmap_iterator& _another) const
{
    if (subarray_ptr < _another.subarray_ptr)
    {
        return true;
    }
    else if (subarray_ptr == _another.subarray_ptr)
    {
        if (index < _another.index)
        {
            return true;
        }
    }

    return false;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::cmap_iterator&
deque<T, Allocator>::cmap_iterator::operator++ ()
{
    index++;
    if (index == subarray_size)
    {
        index = 0;
        subarray_ptr++;
    }
    return *this;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::cmap_iterator&
deque<T, Allocator>::cmap_iterator::operator-- ()
{
    index--;
    if (index < 0)
    {
        index = subarray_size - 1;
        subarray_ptr--;
    }
    return *this;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::cmap_iterator&
deque<T, Allocator>::cmap_iterator::operator+= (difference_type _diff)
{
    subarray_ptr += (index + _diff) / subarray_size;
    index = (index + _diff) % subarray_size;
    return *this;
}

template <typename T, typename Allocator>
typename deque<T, Allocator>::cmap_iterator::difference_type
deque<T, Allocator>::cmap_iterator::operator- (const cmap_iterator& _another)
{
    // according to EA-STL, this is a fairly clever algorithm since HP STL.
    return subarray_size * (subarray_ptr - _another.subarray_ptr - 1)
           + (index)
           + (subarray_size - _another.index);
}



template <typename T, typename Allocator>
void
swap(deque<T, Allocator> &_d1, deque<T, Allocator> &_d2)
{
    _d1.swap(_d2);
}

} // namespace saber

#endif // deque.hpp
