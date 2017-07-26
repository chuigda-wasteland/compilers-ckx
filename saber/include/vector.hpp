#ifdef _MSC_VER
#pragma once
#endif

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include "iterator.hpp"
#include "memory.hpp"
#include "strings.defs.h"
#include "algorithm.hpp"

namespace saber
{

template <typename T,
          typename Allocator = default_allocator<T>>
class vector
{
    static_assert(std::is_copy_constructible<T>::value,
                  C8_STAT__TEMPLATE_ARG__T__COPY_CONSTRUCT_ERROR);
    static_assert(std::is_destructible<T>::value,
                  C8_STAT__TEMPLATE_ARG__T__DESTROY_ERROR);

public:
    using value_type      = T;
    using pointer         = T*;
    using reference       = T&;
    using const_reference = const T&;
    using size_type       = size_t;
    using difference_type = std::ptrdiff_t;
    using allocator_type  = Allocator;

    class iterator;
    class const_iterator;

    using reverse_iterator       = saber::reverse_iterator<iterator>;
    using const_reverse_iterator = saber::reverse_iterator<const_iterator>;

    vector();
    explicit vector(const Allocator& _alloc);
    vector(const vector& _another);
    vector(const vector& _another, const allocator_type& _alloc);
    explicit vector(size_type _n,
                    const allocator_type& _alloc = Allocator());
    explicit vector(size_type _n,
                    const value_type& _value,
                    const allocator_type& _alloc = Allocator());
    vector(vector&& _another);
    vector(vector&& _another, const allocator_type& _alloc);

    vector(initializer_list<value_type> _list,
           const allocator_type& _alloc = Allocator());

    template <typename InputIterator>
    vector(InputIterator _first,
           InputIterator _last,
           const allocator_type& _alloc = Allocator());
    ~vector();

    vector& operator= (const vector& _another);
    vector& operator= (vector&& _another);

    void assign(size_type _n, const value_type& _value);
    void assign(initializer_list<value_type> _list);
    template <typename InputIterator>
    void assign(InputIterator _begin, InputIterator _end);

    size_type size() const;
    size_type max_size() const;
    size_type capacity() const;
    bool empty() const;

    void shink_to_fit(void);
    void reserve(size_type _capacity);
    void push_back(const T& _element);
    void pop_back(void);
    template <typename... Args>
    void emplace_back(Args&&... _args);
    void clear(void);

    iterator insert(const_iterator _position, const value_type& _value);
    iterator insert(const_iterator _position, value_type&& _value);
    iterator insert(const_iterator _position,
                    size_type _n,
                    const value_type& _value);
    template <typename InputIterator>
    iterator insert(const_iterator _position,
                    InputIterator _begin,
                    InputIterator _end);
    iterator insert(const_iterator _position,
                    initializer_list<value_type> _list);

    template <typename... Args>
    iterator emplace(const_iterator _position, Args&&... _args);

    iterator erase(const_iterator _position);
    iterator erase(const_iterator _begin, const_iterator _end);

    reverse_iterator erase(const_reverse_iterator _position);
    reverse_iterator erase(const_reverse_iterator _begin,
                           const_reverse_iterator _end);

    void swap(vector& _another) noexcept;

    reference at(size_type _index);
    reference operator[] (size_type _index);
    const_reference at(size_type _index) const;
    const_reference operator[] (size_type _index) const;

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

private:
    void update_vector();
    void reallocate(size_type _capacity);

    void clear_elements();
    void clear_capacity();

    template <typename U>
    void check_iterator(const U& _it);

    Allocator alloc;
    T *array;
    size_type capacity_val;
    size_type size_val;
    size_t update_count = 0;
    saber_ptr<bool> validating_ptr;
};

template <typename T, typename Allocator>
class vector<T, Allocator>::iterator
{
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = typename vector::value_type;
    using difference_type   = typename vector::difference_type;
    using pointer           = typename vector::pointer;
    using reference         = typename vector::reference;
    using const_reference   = typename vector::const_reference;

    iterator(const iterator&) = default;
    iterator(const typename vector::const_iterator& _const_iter);
    ~iterator() = default;

    reference operator* ();
    reference operator[] (difference_type _n);
    const_reference operator* () const;
    const_reference operator[] (difference_type _n) const;

    iterator& operator+= (difference_type _n);
    iterator& operator-= (difference_type _n);
    iterator& operator++ ();
    iterator& operator-- ();
    iterator operator++(int);
    iterator operator--(int);

    iterator operator+ (difference_type _n) const;
    iterator operator- (difference_type _n) const;
    difference_type operator- (const iterator& _another) const;
    bool operator< (const iterator& _another) const;
    bool operator> (const iterator& _another) const;
    bool operator<= (const iterator& _another) const;
    bool operator>= (const iterator& _another) const;
    bool operator== (const iterator& _another) const;
    bool operator!= (const iterator& _another) const;

private:
    friend class vector;

    iterator(vector* _from, value_type* _ptr) :
        get_from(_from),
        ptr(_ptr),
        update_count(_from->update_count),
        validating_ptr(_from->validating_ptr)
    {}

    void version_check() const;
    void boundary_check(difference_type _offset) const;

    vector *get_from;
    value_type *ptr;
    size_t update_count;
    saber_ptr<bool> validating_ptr;
};

template <typename T, typename Allocator>
class vector<T, Allocator>::const_iterator
{
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = typename vector::value_type;
    using difference_type   = typename vector::difference_type;
    using pointer           = typename vector::pointer;
    using reference         = typename vector::const_reference;
    using const_reference   = typename vector::const_reference;

    const_iterator(const const_iterator&) = default;
    const_iterator(const typename vector::iterator& _mutable_iter);
    ~const_iterator() = default;

    const_reference operator* ();
    const_reference operator[] (difference_type _index);

    const_iterator& operator+= (difference_type _n);
    const_iterator& operator-= (difference_type _n);
    const_iterator& operator++ ();
    const_iterator& operator-- ();
    const const_iterator operator++ (int);
    const const_iterator operator-- (int);

    const_iterator operator+ (difference_type _n) const;
    const_iterator operator- (difference_type _n) const;
    difference_type operator- (const const_iterator& _another) const;

    bool operator< (const const_iterator& _another) const;
    bool operator> (const const_iterator& _another) const;
    bool operator<= (const const_iterator& _another) const;
    bool operator>= (const const_iterator& _another) const;
    bool operator== (const const_iterator& _another) const;
    bool operator!= (const const_iterator& _another) const;

private:
    friend class vector;

    const_iterator(const vector* _from, const value_type* _ptr) :
        get_from(_from),
        ptr(_ptr),
        update_count(_from->update_count),
        validating_ptr(_from->validating_ptr)
    {}

    void version_check() const;
    void boundary_check(difference_type _offset) const;

    const vector *get_from;
    const value_type *ptr;
    size_t update_count;
    saber_ptr<bool> validating_ptr;
};

template <typename T, typename Allocator>
vector<T, Allocator>::vector() :
    vector(Allocator())
{
}

template <typename T, typename Allocator>
vector<T, Allocator>::vector(const Allocator &_alloc) :
    alloc(_alloc),
    validating_ptr(new bool(true))
{
    array = allocator_traits<Allocator>::allocate(alloc, 4);
    capacity_val = 4;
    size_val = 0;

    update_vector();
}

template <typename T, typename Allocator>
vector<T, Allocator>::vector(const vector &_another) :
    vector(_another, _another.alloc)
{
}

template <typename T, typename Allocator>
vector<T, Allocator>::vector(const vector &_another,
                             const allocator_type &_alloc) :
    alloc(_alloc),
    validating_ptr(new bool(true))
{
    stl_warning(C8_DYN__CONT__CONTAINER_COPY);
    array = allocator_traits<Allocator>::allocate(alloc, _another.size());

    saber::uninitialized_copy(_another.begin(), _another.end(), array);

    size_val = capacity_val = _another.size();
    update_vector();
}

template <typename T, typename Allocator>
vector<T, Allocator>::vector(size_type _n,
                             const allocator_type &_alloc) :
    vector(_n, value_type(), _alloc)
{
}

template <typename T, typename Allocator>
vector<T, Allocator>::vector(size_type _n,
                             const value_type &_value,
                             const allocator_type &_alloc) :
    alloc(_alloc),
    validating_ptr(new bool(true))
{
    array = allocator_traits<Allocator>::allocate(alloc, _n);
    capacity_val = _n;
    uninitialized_fill_n(array, _n, _value);
    size_val = _n;
    update_vector();
}

template <typename T, typename Allocator>
vector<T, Allocator>::vector(initializer_list<value_type> _list,
                             const allocator_type &_alloc) :
    vector(_list.begin(), _list.end(), _alloc)
{
}

template <typename T, typename Allocator>
template <typename InputIterator>
vector<T, Allocator>::vector(InputIterator _begin,
                             InputIterator _end,
                             const allocator_type& _alloc) :
    vector(_alloc)
{
    for(; _begin != _end; ++_begin)
    {
        push_back(*_begin);
    }
}

template <typename T, typename Allocator>
vector<T, Allocator>::vector(vector &&_another) :
    alloc(std::move(_another.alloc)),
    validating_ptr(new bool(true))
{
    array = _another.array;
    size_val = _another.size();
    capacity_val = _another.capacity();

    _another.array = nullptr;
    _another.capacity_val = _another.size_val = 0;
    update_vector();
}

template <typename T, typename Allocator>
vector<T, Allocator>::vector(vector &&_another,
                             const allocator_type &_alloc) :
    alloc(_alloc),
    validating_ptr(new bool(true))
{
    array = _another.array;
    _another.array = nullptr;

    size_val = _another.size();
    capacity_val = _another.capacity();

    _another.capacity = _another.size_val = 0;

    update_vector();
}

template <typename T, typename Allocator>
vector<T, Allocator>::~vector()
{
    clear();
    allocator_traits<Allocator>::deallocate(alloc, array, capacity());

    *(validating_ptr.get()) = false;
}


template <typename T, typename Allocator>
vector<T, Allocator>&
vector<T, Allocator>::operator=(const vector &_another)
{
    stl_warning(C8_DYN__CONT__CONTAINER_COPY);

    if (&_another == this)
    {
        return *this;
    }

    clear();
    alloc = _another.alloc;

    if (capacity() < _another.size()) reallocate(_another.size());
    uninitialized_copy(_another.begin(), _another.end(), array);
    size_val = _another.size();
    update_vector();
    return *this;
}

template <typename T, typename Allocator>
vector<T, Allocator>&
vector<T, Allocator>::operator= (vector&& _another)
{
    if (&_another == this)
    {
        return *this;
    }

    clear_elements();
    allocator_traits<Allocator>::deallocate(alloc, array, capacity());

    alloc = std::move(_another.alloc);
    array = _another.array;

    size_val = _another.size();
    capacity_val = _another.capacity();

    _another.array = nullptr;
    _another.size_val = 0;
    _another.capacity_val = 0;

    return *this;
}

template <typename T, typename Allocator>
void
vector<T, Allocator>::assign(size_type _n, const value_type &_value)
{
    clear_elements();

    if (capacity() < _n) reallocate(_n);
    uninitialized_fill_n(array, _n, _value);
    update_vector();
}

template <typename T, typename Allocator>
void
vector<T, Allocator>::assign(initializer_list<value_type> _list)
{
    assign(_list.begin(), _list.end());
}

template <typename T, typename Allocator>
template <typename InputIterator>
void
vector<T, Allocator>::assign(InputIterator _begin, InputIterator _end)
{
    static_assert(traits::is_input_iterator<InputIterator>::value,
                  C8_STAT__TEMPLATE_ARG__ITER__NOT_INPUT_ITERATOR);

    clear_elements();
    for (; _begin != _end; ++_begin) push_back(*_begin);
    update_vector();
}

template <typename T, typename Allocator>
inline typename vector<T, Allocator>::size_type
vector<T, Allocator>::size() const
{
    return size_val;
}

template <typename T, typename Allocator>
inline typename vector<T, Allocator>::size_type
vector<T, Allocator>::max_size() const
{
    return std::numeric_limits<size_type>::max();
}

template <typename T, typename Allocator>
inline typename vector<T, Allocator>::size_type
vector<T, Allocator>::capacity() const
{
    return capacity_val;
}

template <typename T, typename Allocator>
inline bool
vector<T, Allocator>::empty() const
{
    return (size_val == 0);
}

template <typename T, typename Allocator>
inline void
vector<T, Allocator>::shink_to_fit()
{
    // This function makes no actual work
    // However, it manipulates the container according to ISO C++
    // So the container should be "updated"
    update_vector();
}

template <typename T, typename Allocator>
void
vector<T, Allocator>::reserve(typename vector::size_type _capacity)
{
    if (capacity() > _capacity) return;

    T *new_array = allocator_traits<Allocator>::allocate(alloc,_capacity);
    uninitialized_copy(begin(), end(), new_array);

    size_type size_temp = size();
    clear_capacity();

    array = new_array;
    size_val = size_temp;
    capacity_val = _capacity;
    update_vector();
}

template <typename T, typename Allocator>
void
vector<T, Allocator>::push_back(const T &_element)
{
    if (size() == capacity()) reallocate(capacity() * 2);

    construct(&array[size()], _element);
    ++size_val;

    update_vector();
}

template <typename T, typename Allocator>
void
vector<T, Allocator>::pop_back()
{
    destroy_at(&array[size_val-1]);
    --size_val;

    update_vector();
}

template <typename T, typename Allocator>
template <typename... Args>
void
vector<T, Allocator>::emplace_back(Args&&... _args)
{
    if (size() == capacity()) reallocate(capacity() * 2);

    construct(&array[size_val], saber::forward<Args>(_args)...);
    ++size_val;

    update_vector();
}

template <typename T, typename Allocator>
void
vector<T, Allocator>::clear()
{
    erase(cbegin(), cend());
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::insert(const_iterator _position,
                             const value_type &_value)
{
    static_assert(std::is_copy_assignable<T>::value,
                  C8_STAT__TEMPLATE_ARG__T__COPY_ASSIGN_ERROR);
    return insert(_position, size_type(1), _value);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::insert(const_iterator _position,
                             value_type &&_value)
{
    static_assert(std::is_copy_assignable<T>::value,
                  C8_STAT__TEMPLATE_ARG__T__COPY_ASSIGN_ERROR);
    static_assert(std::is_move_assignable<T>::value,
                  C8_STAT__TEMPLATE_ARG__T__MOVE_ASSIGN_ERROR);
    check_iterator(_position);

    return insert(_position, size_type(1), _value);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::insert(const_iterator _position,
                             size_type _n,
                             const value_type &_value)
{
    static_assert(std::is_copy_assignable<T>::value,
                  C8_STAT__TEMPLATE_ARG__T__COPY_ASSIGN_ERROR);
    check_iterator(_position);

    if (empty())
    {
        if (_position == cend())
        {
            for (size_type i = 0; i < _n; ++i) push_back(_value);
            return end()-1;
        }

        stl_panic(C8_DYN__ITER__ITERATOR_OVERFLOW);
    }

    if (size() + _n < capacity())
    {
        push_back(*(cend() - 1));

        iterator iter_temp(this, const_cast<value_type*>(_position.ptr));
        reverse_copy(iter_temp, end() - 1, rbegin());

        fill_n(iter_temp, _n, _value);

        update_vector();
        return iterator(this, iter_temp.ptr);
    }
    else
    {
        size_type add_all_size = size() + _n;
        value_type *new_array =
                allocator_traits<Allocator>::allocate(alloc, add_all_size);
        difference_type position_diff = _position - cbegin();

        uninitialized_copy(cbegin(), cbegin() + position_diff, new_array);
        uninitialized_fill_n(new_array + position_diff, _n, _value);
        uninitialized_copy(cbegin() + position_diff,
                           cend(),
                           new_array + position_diff + _n);

        clear_elements();
        clear_capacity();

        array = new_array;
        size_val = capacity_val = add_all_size;
        update_vector();

        return begin() + position_diff;
    }
}

template <typename T, typename Allocator>
template <typename InputIterator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::insert(const_iterator _position,
                             InputIterator _first,
                             InputIterator _last)
{
    static_assert(std::is_copy_assignable<T>::value,
                  C8_STAT__TEMPLATE_ARG__T__COPY_ASSIGN_ERROR);
    check_iterator(_position);

    difference_type position_diff = _position - cbegin();

    for (; _first != _last; ++_first, ++_position)
    {
        _position = const_iterator(insert(_position, *_first));
    }

    return begin() + position_diff;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::insert(const_iterator _position,
                             initializer_list<value_type> _list)
{
    static_assert(std::is_copy_assignable<T>::value,
                  C8_STAT__TEMPLATE_ARG__T__COPY_ASSIGN_ERROR);
    check_iterator(_position);

    return insert(_position, _list.begin(), _list.end());
}

template <typename T, typename Allocator>
template <typename... Args>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::emplace(const_iterator _position, Args&&... _args)
{

    check_iterator(_position);

    if (empty())
    {
        emplace_back(std::forward<Args>(_args)...);
        return begin();
    }

    if (size() + 1 <= capacity())
    {
        push_back(*(cend() - 1));
        iterator iter_temp(this, const_cast<value_type*>(_position.ptr));

        reverse_copy(iter_temp, end(), rbegin());
        destroy_at(iter_temp.ptr);
        construct(iter_temp.ptr, saber::forward<Args>(_args)...);

        update_vector();
        return iterator(this, iter_temp.ptr);
    }
    else
    {
        size_type add_all_size = size() + 1;
        value_type *new_array =
                allocator_traits<Allocator>::allocate(alloc, add_all_size);
        difference_type position_diff = _position - cbegin();

        uninitialized_copy(cbegin(), cbegin() + position_diff, new_array);
        construct(new_array + position_diff, saber::forward<Args>(_args)...);
        uninitialized_copy(cbegin() + position_diff,
                           cend(),
                           new_array + position_diff + 1);

        clear_elements();
        clear_capacity();

        array = new_array;
        size_val = capacity_val = add_all_size;
        update_vector();

        return begin() + position_diff;
    }
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::erase(const_iterator _position)
{
    check_iterator(_position);

    iterator iter_temp = iterator(_position);
    if (_position + 1 != cend())
    {
        return erase(_position, _position + 1);
    }

    return iter_temp;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::erase(const_iterator _first,
                            const_iterator _last)
{
    check_iterator(_first);
    check_iterator(_last);

    update_vector();

    iterator iter_first(this, const_cast<value_type*>(_first.ptr));
    iterator iter_last(this, const_cast<value_type*>(_last.ptr));

    iterator end_of_storage = copy(iter_last, end(), iter_first);
    destroy(end_of_storage, end());

    size_val = (cend() - (iter_last - iter_first)) - cbegin();
    return iter_first;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::reverse_iterator
vector<T, Allocator>::erase(const_reverse_iterator _position)
{
    check_iterator(_position.base());

    return reverse_iterator(erase(_position.base()));
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::reverse_iterator
vector<T, Allocator>::erase(const_reverse_iterator _first,
                            const_reverse_iterator _last)
{
    static_assert(std::is_move_assignable<T>::value,
                  C8_STAT__TEMPLATE_ARG__T__MOVE_ASSIGN_ERROR);

    check_iterator(_first.base());
    check_iterator(_last.base());

    return reverse_iterator(erase(_last.base(), _first.base()));
}

template <typename T, typename Allocator>
void
vector<T, Allocator>::swap(vector& _another) noexcept
{
    saber::swap(array, _another.array);
    saber::swap(alloc, _another.alloc);
    saber::swap(size_val, _another.size_val);
    saber::swap(capacity_val, _another.capacity_val);
    saber::swap(update_count, _another.update_count);
    saber::swap(validating_ptr, _another.validating_ptr);

    update_vector();
    _another.update_vector();
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::reference
vector<T, Allocator>::at(typename vector::size_type _index)
{
    if (_index >= size())
        throw std::out_of_range(C8_DYN__ITER__SUBSCRIPT_OVERFLOW);
    return array[_index];
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_reference
vector<T, Allocator>::at(typename vector::size_type _index) const
{
    if (_index >= size())
        throw std::out_of_range(C8_DYN__ITER__SUBSCRIPT_OVERFLOW);
    return array[_index];
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::reference
vector<T, Allocator>::operator[](typename vector::size_type _index)
{
    if (_index >= size()) stl_panic(C8_DYN__ITER__SUBSCRIPT_OVERFLOW);
    return array[_index];
}



template <typename T, typename Allocator>
typename vector<T, Allocator>::const_reference
vector<T, Allocator>::operator[](typename vector::size_type _index) const
{
    if (_index >= size()) stl_panic(C8_DYN__ITER__SUBSCRIPT_OVERFLOW);
    return array[_index];
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::begin() noexcept
{
    return iterator(this, &array[0]);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::end() noexcept
{
    return iterator(this, &array[size()]);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator
vector<T, Allocator>::begin() const noexcept
{
    return const_iterator(this, &array[0]);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator
vector<T, Allocator>::end() const noexcept
{
    return const_iterator(this, &array[size()]);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator
vector<T, Allocator>::cbegin() const noexcept
{
    return const_iterator(this, &array[0]);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator
vector<T, Allocator>::cend() const noexcept
{
    return const_iterator(this, &array[size()]);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::reverse_iterator
vector<T, Allocator>::rbegin() noexcept
{
    return reverse_iterator(end());
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::reverse_iterator
vector<T, Allocator>::rend() noexcept
{
    return reverse_iterator(begin());
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_reverse_iterator
vector<T, Allocator>::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_reverse_iterator
vector<T, Allocator>::rend() const noexcept
{
    return const_reverse_iterator(begin());
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_reverse_iterator
vector<T, Allocator>::crbegin() const noexcept
{
    return const_reverse_iterator(rend());
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_reverse_iterator
vector<T, Allocator>::crend() const noexcept
{
    return const_reverse_iterator(rbegin());
}

template <typename T, typename Allocator>
void
vector<T, Allocator>::update_vector()
{
    if (update_count != std::numeric_limits<size_t>::max())
    {
        update_count++;
    }
    else
    {
        update_count = 0;
    }
}

template <typename T, typename Allocator>
void
vector<T, Allocator>::reallocate(size_type _capacity)
{
    value_type *new_array =
            allocator_traits<Allocator>::allocate(alloc, _capacity);
    std::uninitialized_copy(begin(), end(), new_array);

    size_type size_temp = size();
    clear_elements();
    clear_capacity();

    array = new_array;
    capacity_val = _capacity;
    size_val = size_temp;
}

template <typename T, typename Allocator>
template <typename U>
void
vector<T, Allocator>::check_iterator(const U& _it)
{
    if (_it.get_from != this)
    {
        stl_panic(C8_DYN__ITER__UNKNOWN_REGION_ITERATOR);
    }
    _it.version_check();
}

template <typename T, typename Allocator>
void vector<T, Allocator>::clear_elements()
{
    for (size_t i = 0; i < size(); i++)
    {
        destroy_at(array + i);
    }
}

template <typename T, typename Allocator>
void vector<T, Allocator>::clear_capacity()
{
    allocator_traits<Allocator>::deallocate(alloc, array, capacity());
    capacity_val = 0;
    size_val = 0;
}



template <typename T, typename Allocator>
vector<T, Allocator>::iterator::iterator(
        const typename vector::const_iterator& _const_iter) :
    iterator(const_cast<vector*>(_const_iter.get_from),
             const_cast<value_type*>(_const_iter.ptr))
{}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator::reference
vector<T, Allocator>::iterator::operator* ()
{
    return operator[](0);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator::reference
vector<T, Allocator>::iterator::operator[](difference_type _n)
{
    version_check();
    boundary_check(_n);
    return *(ptr+_n);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator::const_reference
vector<T, Allocator>::iterator::operator* () const
{
    return operator[](0);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator::const_reference
vector<T, Allocator>::iterator::operator[](difference_type _n) const
{
    version_check();
    boundary_check(_n);
    return const_cast<const_reference>(*(ptr+_n));
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator::difference_type
vector<T, Allocator>::iterator::operator-
    (const iterator &_another) const
{
    return ptr - _another.ptr;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::iterator::operator+(difference_type _n) const
{
    return iterator(get_from, ptr + _n);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::iterator::operator-(difference_type _n) const
{
    return iterator(get_from, ptr - _n);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator&
vector<T, Allocator>::iterator::operator+=(difference_type _n)
{
    version_check();
    ptr += _n;
    return *this;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator&
vector<T, Allocator>::iterator::operator-=(difference_type _n)
{
    version_check();
    ptr -= _n;
    return *this;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator&
vector<T, Allocator>::iterator::operator++()
{
    version_check();
    ++ptr;
    return *this;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator&
vector<T, Allocator>::iterator::operator--()
{
    version_check();
    --ptr;
    return *this;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::iterator::operator++(int)
{
    version_check();
    iterator ret = *this;
    ++ptr;
    return ret;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::iterator
vector<T, Allocator>::iterator::operator--(int)
{
    version_check();
    iterator ret = *this;
    --ptr;
    return ret;
}

template <typename T, typename Allocator>
bool
vector<T, Allocator>::iterator::operator<
    (const iterator &_another) const
{
    return (ptr < _another.ptr);
}

template <typename T, typename Allocator>
bool
vector<T, Allocator>::iterator::operator>
    (const iterator &_another) const
{
    return (ptr > _another.ptr);
}

template <typename T, typename Allocator>
bool
vector<T, Allocator>::iterator::operator<=
    (const iterator &_another) const
{
    return ! operator>(_another);
}

template <typename T, typename Allocator>
bool
vector<T, Allocator>::iterator::operator>=
    (const iterator &_another) const
{
    return ! operator<(_another);
}

template <typename T, typename Allocator>
bool
vector<T, Allocator>::iterator::operator==
    (const iterator &_another) const
{
    return (ptr == _another.ptr);
}

template <typename T, typename Allocator>
bool
vector<T, Allocator>::iterator::operator!=
    (const iterator &_another) const
{
    return ! operator==(_another);
}



template <typename T, typename Allocator>
vector<T, Allocator>::const_iterator::const_iterator(
        const typename vector::iterator& _mutable_iter) :
    const_iterator(_mutable_iter.get_from, _mutable_iter.ptr)
{}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator::const_reference
vector<T, Allocator>::const_iterator::operator* ()
{
    return operator[](0);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator::const_reference
vector<T, Allocator>::const_iterator::operator[] (difference_type _n)
{
    version_check();
    boundary_check(_n);
    return *(ptr+_n);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator&
vector<T, Allocator>::const_iterator::operator+= (difference_type _n)
{
    version_check();

    ptr += _n;
    return *this;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator&
vector<T, Allocator>::const_iterator::operator-= (difference_type _n)
{
    version_check();
    ptr -= _n;
    return *this;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator&
vector<T, Allocator>::const_iterator::operator++ ()
{
    version_check();
    ++ptr;
    return *this;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator&
vector<T, Allocator>::const_iterator::operator--()
{
    version_check();
    ++ptr;
    return *this;
}

template <typename T, typename Allocator>
const typename vector<T, Allocator>::const_iterator
vector <T, Allocator>::const_iterator::operator++(int)
{
    version_check();
    iterator ret = *this;
    ++ptr;
    return ret;
}

template <typename T, typename Allocator>
const typename vector<T, Allocator>::const_iterator
vector <T, Allocator>::const_iterator::operator--(int)
{
    version_check();
    iterator ret = *this;
    --ptr;
    return ret;
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator
vector<T, Allocator>::const_iterator::operator+ (difference_type _n) const
{
    return const_iterator(get_from, ptr + _n);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator
vector<T, Allocator>::const_iterator::operator- (difference_type _n) const
{
    return const_iterator(get_from, ptr - _n);
}

template <typename T, typename Allocator>
typename vector<T, Allocator>::const_iterator::difference_type
vector<T, Allocator>::const_iterator::operator-
    (const const_iterator& _another) const
{
    return ptr - _another.ptr;
}

template <typename T, typename Allocator>
bool
vector<T, Allocator>::const_iterator::operator<
    (const const_iterator& _another) const
{
    return (ptr < _another.ptr);
}

template <typename T, typename Allocator>
bool
vector<T, Allocator>::const_iterator::operator>
    (const const_iterator& _another) const
{
    return (ptr > _another.ptr);
}

template <typename T, typename Allocator>
bool
vector<T, Allocator>::const_iterator::operator<=
    (const const_iterator& _another) const
{
    return ! operator>(_another);
}

template <typename T, typename Allocator>
bool
vector<T, Allocator>::const_iterator::operator>=
    (const const_iterator& _another) const
{
    return ! operator<(_another);
}

template <typename T, typename Allocator>
bool
vector<T, Allocator>::const_iterator::operator==
    (const const_iterator& _another) const
{
    return (ptr == _another.ptr);
}

template <typename T, typename Allocator>
bool
vector<T, Allocator>::const_iterator::operator!=
    (const const_iterator& _another) const
{
    return ! operator==(_another);
}

template <typename T, typename Allocator>
void
vector<T, Allocator>::iterator::version_check() const
{
    if (*(validating_ptr.get()) == false)
    {
        stl_panic(C8_DYN__ITER__DELETED_CONTAINER);
    }

    if (update_count != get_from->update_count)
    {
        stl_panic(C8_DYN__ITER__OLD_ITERATOR);
    }
}

template <typename T, typename Allocator>
void
vector<T, Allocator>::iterator::boundary_check(difference_type _offset) const
{
    if (ptr + _offset >= &(get_from->array[get_from->size()])
            || ptr + _offset < &(get_from->array[0]))
    {
        stl_panic(C8_DYN__ITER__ITERATOR_OVERFLOW);
    }
}

template <typename T, typename Allocator>
void
vector<T, Allocator>::const_iterator::version_check() const
{
    if (*(validating_ptr.get()) == false)
    {
        stl_panic(C8_DYN__ITER__DELETED_CONTAINER);
    }

    if (update_count != get_from->update_count)
    {
        stl_panic(C8_DYN__ITER__OLD_ITERATOR);
    }
}

template <typename T, typename Allocator>
void
vector<T, Allocator>::const_iterator::boundary_check(difference_type _offset) const
{
    if (ptr + _offset >= &(get_from->array[get_from->size()])
            || ptr + _offset < &(get_from->array[0]))
    {
        stl_panic(C8_DYN__ITER__ITERATOR_OVERFLOW);
    }
}

template <typename T, typename Allocator>
void
swap(vector<T, Allocator> &_a, vector<T, Allocator> &_b)
{
    _a.swap(_b);
}

} // namespace saber

#endif // VECTOR_HPP
