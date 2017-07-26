#ifndef SET_HPP
#define SET_HPP

#include "safe_stl_general.hpp"
#include "functional.hpp"
#include "memory.hpp"
#include "free_tree.hpp"
#include "iterator.hpp"
#include "utility.hpp"

namespace saber
{

template <typename Key,
          typename Compare = less<Key>,
          typename Allocator = default_allocator<Key>>
class set
{
public:
    using key_type        = Key;
    using value_type      = Key;
    using key_compare     = Compare;
    using value_compare   = Compare;
    using allocator_type  = Allocator;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using size_type       = size_t;
    using difference_type = std::ptrdiff_t;

    class iterator;
    class const_iterator;

    using reverse_iterator       = saber::reverse_iterator<iterator>;
    using const_reverse_iterator = saber::reverse_iterator<const_iterator>;

    using pointer       = typename allocator_traits<Allocator>::pointer;
    using const_pointer = typename allocator_traits<Allocator>::const_pointer;

    explicit set(const Compare& _comp = Compare(),
                 const Allocator& _alloc = Allocator());
    template <typename InputIterator>
    set(InputIterator _first, InputIterator _last,
        const Compare& _comp = Compare(), 
        const Allocator& _alloc = Allocator());
    set(const set& _another);
    set(const set& _another, const Allocator& _alloc);
    set(set&& _another);
    set(set&& _another, const Allocator& _alloc);
    set(std::initializer_list<value_type> _ilist,
        const Compare& _comp = Compare(),
        const Allocator& _alloc = Allocator());
    
    ~set();

    set& operator= (const set& _another);
    set& operator= (set&& _another);
    set& operator= (std::initializer_list<value_type> _ilist);

    allocator_type get_allocator() const;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    reverse_iterator rbegin() noexcept;
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;

    reverse_iterator rend() noexcept;
    const_reverse_iterator rend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    size_type size() const noexcept;
    size_type max_size() const noexcept;
    bool empty() const noexcept;

    template <typename... Args>
    pair<iterator, bool> emplace(Args&& ...args);
    template <typename... Args>
    iterator emplace_hint(const_iterator _pos, Args&& ...args);

    pair<iterator, bool> insert(const value_type& _value);
    pair<iterator, bool> insert(value_type&& _value);
    iterator insert(const_iterator _pos, const value_type& _value);
    iterator insert(const_iterator _pos, value_type&& _value);
    template <typename InputIterator>
    void insert(InputIterator _first, InputIterator _last);
    void insert(initializer_list<value_type> _ilist);

    iterator erase(const_iterator _pos);
    iterator erase(const_iterator _first, const_iterator _last);
    size_type erase(const value_type& _value);

    void swap(set& _another);
    void clear() noexcept;
    
    key_compare key_comp() const;
    value_compare value_comp() const;

    iterator find(const key_type& _key);
    const_iterator find(const key_type& _key) const;

    size_type count(const key_type& _key) const;
    
    iterator lower_bound(const key_type& _key);
    const_iterator lower_bound(const key_type& _key) const;
    iterator upper_bound(const key_type& _key);
    const_iterator upper_bound(const key_type& _key) const;

private:
    using tree_type = free_tree<Key, Compare, Allocator>;
    using tree_iterator = typename tree_type::tree_iterator;

    tree_type *p_tree_impl;
}; // class saber::set



template <typename Key, typename Compare, typename Allocator>
class set<Key, Compare, Allocator>::iterator
{
    friend class set;
    friend class set::const_iterator;
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = typename set::value_type;
    using pointer           = typename set::pointer;
    using reference         = typename set::reference;
    using const_reference   = typename set::const_reference;
    using size_type         = typename set::size_type;
    using difference_type   = typename set::difference_type;

    explicit iterator() = default;
    iterator(const typename set::const_iterator &_const_iter);
    ~iterator() = default;

    reference operator* (void);
    const_reference operator* (void) const;

    iterator& operator++(void);
    iterator& operator--(void);
    iterator operator++(int);
    iterator operator--(int);

    bool operator== (const iterator& _another) const;
    bool operator!= (const iterator& _another) const;

private:
    using tree_iterator = typename set::tree_iterator;

    iterator(const tree_iterator& _actual_iter, const set* _get_from) :
        actual_iter(_actual_iter), get_from(_get_from) {}

    tree_iterator actual_iter;
    const set *get_from = nullptr;
};

template <typename Key, typename Compare, typename Allocator>
class set<Key, Compare, Allocator>::const_iterator
{
    friend class set;
    friend class set::iterator;
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = typename set::value_type;
    using pointer           = typename set::pointer;
    using reference         = typename set::reference;
    using const_reference   = typename set::const_reference;
    using size_type         = typename set::size_type;
    using difference_type   = typename set::difference_type;

    explicit const_iterator() = default;
    const_iterator(const set::iterator& _mutable_iter);
    ~const_iterator() = default;

    const_reference operator* (void) const;

    const_iterator& operator++ (void);
    const_iterator& operator-- (void);
    const_iterator operator++ (int);
    const_iterator operator-- (int);

    bool operator== (const const_iterator& _another) const;
    bool operator!= (const const_iterator& _another) const;

private:
    using tree_iterator = typename set::tree_iterator;

    const_iterator(const tree_iterator& _actual_iter, const set* _get_from) :
        actual_iter(_actual_iter), get_from(_get_from) {}

    tree_iterator actual_iter;
    const set *get_from = nullptr;
};



template <typename Key, typename Compare, typename Allocator>
set<Key, Compare, Allocator>::set(const Compare& _comp, const Allocator& _alloc)
{
    p_tree_impl = new tree_type(_comp, _alloc);
}

template <typename Key, typename Compare, typename Allocator>
template <typename InputIterator>
set<Key, Compare, Allocator>::set(InputIterator _first,
                                  InputIterator _last,
                                  const Compare& _comp,
                                  const Allocator& _alloc) :
    set(_comp, _alloc)
{
    for (; _first != _last; ++_first)
    {
        insert(*_first);
    }
}

template <typename Key, typename Compare, typename Allocator>
set<Key, Compare, Allocator>::set(std::initializer_list<value_type> _ilist,
                                  const Compare &_comp,
                                  const Allocator &_alloc) :
    set(_ilist.begin(), _ilist.end(), _comp, _alloc)
{}

template <typename Key, typename Compare, typename Allocator>
set<Key, Compare, Allocator>::set(const set &_another) :
    set(_another.cbegin(), _another.cend(),
        _another.key_comp(), _another.get_allocator())
{}

template <typename Key, typename Compare, typename Allocator>
set<Key, Compare, Allocator>::set(set &&_another)
{
    p_tree_impl = _another.p_tree_impl;
    _another.p_tree_impl = nullptr;
}

template <typename Key, typename Compare, typename Allocator>
set<Key, Compare, Allocator>::set(const set &_another,
                                  const Allocator &_alloc) :
    set(_another.begin(), _another.end(), _another.key_comp(), _alloc)
{}

template <typename Key, typename Compare, typename Allocator>
set<Key, Compare, Allocator>::set(set &&_another, const Allocator &_alloc) :
    set(_another.begin(), _another.end(), _another.key_comp(), _alloc)
{}


template <typename Key, typename Compare, typename Allocator>
set<Key, Compare, Allocator>::~set()
{
    delete p_tree_impl;
}



template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::allocator_type
set<Key, Compare, Allocator>::get_allocator() const
{
    return p_tree_impl->get_allocator();
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::iterator
set<Key, Compare, Allocator>::begin() noexcept
{
    return iterator(p_tree_impl->begin(), this);
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::iterator
set<Key, Compare, Allocator>::end() noexcept
{
    return iterator(p_tree_impl->end(), this);
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::const_iterator
set<Key, Compare, Allocator>::begin() const noexcept
{
    return const_iterator(p_tree_impl->begin(), this);
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::const_iterator
set<Key, Compare, Allocator>::end() const noexcept
{
    return const_iterator(p_tree_impl->end(), this);
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::const_iterator
set<Key, Compare, Allocator>::cbegin() const noexcept
{
    return begin();
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::const_iterator
set<Key, Compare, Allocator>::cend() const noexcept
{
    return end();
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::reverse_iterator
set<Key, Compare, Allocator>::rbegin() noexcept
{
    return reverse_iterator(end());
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::reverse_iterator
set<Key, Compare, Allocator>::rend() noexcept
{
    return reverse_iterator(begin());
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::const_reverse_iterator
set<Key, Compare, Allocator>::rbegin() const noexcept
{
    return reverse_iterator(cend());
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::const_reverse_iterator
set<Key, Compare, Allocator>::rend() const noexcept
{
    return reverse_iterator(cbegin());
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::const_reverse_iterator
set<Key, Compare, Allocator>::crbegin() const noexcept
{
    return rbegin();
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::const_reverse_iterator
set<Key, Compare, Allocator>::crend() const noexcept
{
    return rend();
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::size_type
set<Key, Compare, Allocator>::size() const noexcept
{
    return distance(cbegin(), cend());
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::size_type
set<Key, Compare, Allocator>::max_size() const noexcept
{
    return std::numeric_limits<size_type>::max();
}

template <typename Key, typename Compare, typename Allocator>
bool
set<Key, Compare, Allocator>::empty() const noexcept
{
    return cbegin() == cend();
}

template <typename Key, typename Compare, typename Allocator>
template <typename... Args>
pair<typename set<Key, Compare, Allocator>::iterator, bool>
set<Key, Compare, Allocator>::emplace(Args&& ...args)
{
    auto result = p_tree_impl->emplace(saber::forward<Args>(args)...);
    return pair<iterator, bool>(iterator(result.first, this), result.second);
}

template <typename Key, typename Compare, typename Allocator>
template <typename... Args>
typename set<Key, Compare, Allocator>::iterator
set<Key, Compare, Allocator>::emplace_hint(const_iterator, Args&& ...args)
{
    return emplace(saber::forward<Args>(args)...).first;
}

template <typename Key, typename Compare, typename Allocator>
pair<typename set<Key, Compare, Allocator>::iterator, bool>
set<Key, Compare, Allocator>::insert(const value_type &_value)
{
    auto result = p_tree_impl->insert(_value);
    return pair<iterator, bool>(iterator(result.first, this), result.second);
}

template <typename Key, typename Compare, typename Allocator>
pair<typename set<Key, Compare, Allocator>::iterator, bool>
set<Key, Compare, Allocator>::insert(value_type &&_value)
{
    auto result = p_tree_impl->insert(std::move(_value));
    return pair<iterator, bool>(iterator(result.first, this), result.second);
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::iterator
set<Key, Compare, Allocator>::insert(const_iterator, const value_type &_value)
{
    return insert(_value).first;
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::iterator
set<Key, Compare, Allocator>::insert(const_iterator, value_type &&_value)
{
    return insert(std::move(_value)).first;
}

template <typename Key, typename Compare, typename Allocator>
template <typename InputIterator>
void
set<Key, Compare, Allocator>::insert(InputIterator _first, InputIterator _last)
{
    for (; _first != _last; ++_first) insert(*_first);
}

template <typename Key, typename Compare, typename Allocator>
void
set<Key, Compare, Allocator>::insert(initializer_list<value_type> _ilist)
{
    insert(_ilist.begin(), _ilist.end());
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::iterator
set<Key, Compare, Allocator>::erase(const_iterator _pos)
{
    iterator ret = _pos;
    ++ret;
    p_tree_impl->erase(_pos.actual_iter);
    return ret;
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::iterator
set<Key, Compare, Allocator>::erase(const_iterator _first, const_iterator _last)
{
    while (_first != _last)
    {
        const_iterator it = _first++;
        erase(it);
    }
    return iterator(_last);
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::size_type
set<Key, Compare, Allocator>::erase(const value_type &_value)
{
    const_iterator erase_pos = const_cast<const set*>(this)->find(_value);

    if (erase_pos == cend())
        return 0;
    erase(erase_pos);
    return 1;
}

template <typename Key, typename Compare, typename Allocator>
void
set<Key, Compare, Allocator>::swap(set &_another)
{
    swap(p_tree_impl, _another.p_tree_impl);
}

template <typename Key, typename Compare, typename Allocator>
void
set<Key, Compare, Allocator>::clear() noexcept
{
    p_tree_impl->clear();
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::key_compare
set<Key, Compare, Allocator>::key_comp() const
{
    return p_tree_impl->value_comp();
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::value_compare
set<Key, Compare, Allocator>::value_comp() const
{
    return p_tree_impl->value_comp();
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::iterator
set<Key, Compare, Allocator>::find(const key_type &_key)
{
    return iterator(p_tree_impl->find(_key), this);
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::const_iterator
set<Key, Compare, Allocator>::find(const key_type &_key) const
{
    return const_iterator(p_tree_impl->find(_key), this);
}



template <typename Key, typename Compare, typename Allocator>
set<Key, Compare, Allocator>::iterator::iterator(
        const typename set::const_iterator& _const_iter) :
    iterator(_const_iter.actual_iter, _const_iter.get_from)
{}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::iterator::reference
set<Key, Compare, Allocator>::iterator::operator* ()
{
    return *actual_iter;
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::iterator::const_reference
set<Key, Compare, Allocator>::iterator::operator* () const
{
    return *actual_iter;
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::iterator&
set<Key, Compare, Allocator>::iterator::operator++ ()
{
    ++actual_iter;
    return *this;
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::iterator&
set<Key, Compare, Allocator>::iterator::operator--()
{
    --actual_iter;
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::iterator
set<Key, Compare, Allocator>::iterator::operator++ (int)
{
    iterator ret = *this;
    operator++();
    return ret;
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::iterator
set<Key, Compare, Allocator>::iterator::operator-- (int)
{
    iterator ret = *this;
    operator--();
    return ret;
}

template <typename Key, typename Compare, typename Allocator>
bool
set<Key, Compare, Allocator>::iterator::operator== (
    const iterator& _another) const
{
    return actual_iter == _another.actual_iter;
}

template <typename Key, typename Compare, typename Allocator>
bool
set<Key, Compare, Allocator>::iterator::operator!= (
    const iterator& _another) const
{
    return !operator== (_another);
}



template <typename Key, typename Compare, typename Allocator>
set<Key, Compare, Allocator>::const_iterator::const_iterator(
        const set::iterator& _mutable_iterator) :
    const_iterator(_mutable_iterator.actual_iter, _mutable_iterator.get_from)
{}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::const_iterator::const_reference
set<Key, Compare, Allocator>::const_iterator::operator* () const
{
    return *actual_iter;
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::const_iterator&
set<Key, Compare, Allocator>::const_iterator::operator++ ()
{
    ++actual_iter;
    return *this;
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::const_iterator&
set<Key, Compare, Allocator>::const_iterator::operator-- ()
{
    --actual_iter;
    return *this;
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::const_iterator
set<Key, Compare, Allocator>::const_iterator::operator++ (int)
{
    const_iterator ret = *this;
    operator++();
    return ret;
}

template <typename Key, typename Compare, typename Allocator>
typename set<Key, Compare, Allocator>::const_iterator
set<Key, Compare, Allocator>::const_iterator::operator-- (int)
{
    const_iterator ret = *this;
    operator--();
    return ret;
}

template <typename Key, typename Compare, typename Allocator>
bool
set<Key, Compare, Allocator>::const_iterator::operator== (
    const const_iterator& _another) const
{
    return actual_iter == _another.actual_iter;
}

template <typename Key, typename Compare, typename Allocator>
bool
set<Key, Compare, Allocator>::const_iterator::operator!= (
    const const_iterator& _another) const
{
    return !operator== (_another);
}

} // namespace saber

#endif // MAP_HPP
