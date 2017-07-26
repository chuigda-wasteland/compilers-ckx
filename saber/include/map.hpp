#ifndef MAP_HPP
#define MAP_HPP

#include "safe_stl_general.hpp"
#include "functional.hpp"
#include "memory.hpp"
#include "free_tree.hpp"
#include "iterator.hpp"

namespace saber
{

template <typename Key,
          typename T,
          typename Compare = less<Key>,
          typename Allocator = default_allocator<Key>>
class map
{
public:
    using key_type        = Key;
    using mapped_type     = T;
    using value_type      = pair<const Key, T>;
    using key_compare     = Compare;
    using allocator_type  = Allocator;
    using reference       = value_type&;
    using const_reference = const value_type&;

    class iterator;
    class const_iterator;

    using reverse_iterator       = saber::reverse_iterator<iterator>;
    using const_reverse_iterator = saber::reverse_iterator<const_iterator>;

    using size_type       = size_t;
    using difference_type = std::ptrdiff_t;

    using pointer       = typename allocator_traits<Allocator>::pointer;
    using const_pointer = typename allocator_traits<Allocator>::const_pointer;

    class value_compare
    {
        friend class map;
    protected:
        Compare comp;
        value_compare(Compare _comp) : comp(_comp) {}

    public:
        using result_type = bool;
        using first_argument_type = value_type;
        using second_argument_type = value_type;

        bool operator()(const value_type& _x, const value_type& _y) const
        { return comp(_x.first, _y.first); }
    };

    explicit map(const Compare& _comp = Compare(),
                 const Allocator& _alloc = Allocator());
    template <typename InputIterator>
    map(InputIterator _first, InputIterator _last,
        const Compare& _comp = Compare(),
        const Allocator& _alloc = Allocator());
    map(const map& _another);
    map(map&& _another);
    map(const map& _another, const Allocator& _alloc);
    map(map&& _another, const Allocator& _alloc);
    map(initializer_list<value_type> _ilist,
        const Compare& _comp = Allocator(),
        const Allocator& _alloc = Allocator());
    ~map();

    map& operator= (const map& _another);
    map& operator= (map&& _another);
    allocator_type get_allocator() const noexcept;

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

    bool empty() const noexcept;
    size_type size() const noexcept;
    size_type max_size() const noexcept;

    T& operator[] (const key_type& _key);
    T& operator[] (key_type&& _key);

    T& at(const key_type& _key);
    const T& at(const key_type &_key) const;

    template <typename... Args>
    pair<iterator, bool> emplace(Args&& ..._args);
    template <typename... Args>
    iterator emplace_hint(const_iterator _pos, Args&& ..._args);

    pair<iterator, bool> insert(const value_type& _pair);
    iterator insert(const_iterator _pos, const value_type& _pair);
//    template <typename Pair>
//    pair<iterator, bool> insert(Pair&& _pair);
//    template <typename Pair> iterator
//    insert(const_iterator _pos, Pair&& _pair);

    template <typename InputIterator>
    void insert(InputIterator _first, InputIterator _last);
    void insert(initializer_list<value_type> _ilist);

    iterator erase(const_iterator _pos);
    size_type erase(const key_type& _x);
    iterator erase(const_iterator _first, const_iterator _last);

    void swap(map& _another);
    void clear() noexcept;

    key_compare key_comp() const;
    value_compare value_comp() const;

    iterator find(const key_type& _x);
    const_iterator find(const key_type &_x) const;
    size_type count(const key_type& _x) const;

    iterator lower_bound(const key_type& _x);
    const_iterator lower_bound(const key_type& _x) const;
    iterator upper_bound(const key_type& _x);
    const_iterator upper_bound(const key_type &_x) const;

    pair<iterator, iterator> equal_range(const key_type& _x);
    pair<const_iterator, const_iterator> equal_range(const key_type &_x) const;

private:
    using tree_type = free_tree<value_type, value_compare, Allocator>;
    using tree_iterator = typename tree_type::tree_iterator;

    tree_type *p_tree_impl;
}; // class map



template <typename Key, typename T, typename Compare, typename Allocator>
class map<Key, T, Compare, Allocator>::iterator
{
    friend class map;
    friend class map::const_iterator;
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename map::value_type;
    using poinrer = typename map::pointer;
    using reference = typename map::reference;
    using const_reference = typename map::const_reference;
    using size_type = typename map::size_type;
    using difference_type = typename map::difference_type;

    explicit iterator() = default;
    iterator(const typename map::const_iterator& _const_iter);
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
    using tree_iterator = typename map::tree_iterator;

    iterator(const tree_iterator& _actual_iter, const map* _get_from) :
        actual_iter(_actual_iter), get_from(_get_from) {}

    tree_iterator actual_iter;
    const map *get_from = nullptr;
};



template <typename Key, typename T, typename Compare, typename Allocator>
class map<Key, T, Compare, Allocator>::const_iterator
{
    friend class map;
    friend class map::iterator;
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename map::value_type;
    using pointer = typename map::pointer;
    using reference = typename map::reference;
    using const_reference = typename map::const_reference;
    using size_type = typename map::size_type;
    using difference_type = typename map::difference_type;

    explicit const_iterator() = default;
    const_iterator(const map::iterator& _mutable_iter);
    ~const_iterator() = default;

    const_reference operator* (void) const;

    const_iterator& operator++ (void);
    const_iterator& operator-- (void);
    const_iterator operator++ (int);
    const_iterator operator-- (int);

    bool operator== (const const_iterator& _another) const;
    bool operator!= (const const_iterator& _another) const;

private:
    using tree_iterator = typename map::tree_iterator;

    const_iterator(const tree_iterator& _actual_iter, const map* get_from) :
        actual_iter(_actual_iter), get_from(get_from) {}

    tree_iterator actual_iter;
    const map *get_from = nullptr;
};



template <typename Key, typename T, typename Compare, typename Allocator>
map<Key, T, Compare, Allocator>::map(const Compare &_comp,
                                     const Allocator &_alloc)
{
    p_tree_impl = new tree_type(value_compare(_comp), _alloc);
}

template <typename Key, typename T, typename Compare, typename Allocator>
map<Key, T, Compare, Allocator>::~map()
{
    delete p_tree_impl;
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::iterator
map<Key, T, Compare, Allocator>::begin() noexcept
{
    return iterator(p_tree_impl->begin(), this);
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::const_iterator
map<Key, T, Compare, Allocator>::begin() const noexcept
{
    return const_iterator(p_tree_impl->begin(), this);
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::const_iterator
map<Key, T, Compare, Allocator>::cbegin() const noexcept
{
    return begin();
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::iterator
map<Key, T, Compare, Allocator>::end() noexcept
{
    return iterator(p_tree_impl->end(), this);
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::const_iterator
map<Key, T, Compare, Allocator>::end() const noexcept
{
    return const_iterator(p_tree_impl->end(), this);
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::const_iterator
map<Key, T, Compare, Allocator>::cend() const noexcept
{
    return end();
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::reverse_iterator
map<Key, T, Compare, Allocator>::rbegin() noexcept
{
    return reverse_iterator(begin());
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::const_reverse_iterator
map<Key, T, Compare, Allocator>::rbegin() const noexcept
{
    return const_reverse_iterator(begin());
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::const_reverse_iterator
map<Key, T, Compare, Allocator>::crbegin() const noexcept
{
    return rbegin();
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::reverse_iterator
map<Key, T, Compare, Allocator>::rend() noexcept
{
    return reverse_iterator(end());
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::const_reverse_iterator
map<Key, T, Compare, Allocator>::rend() const noexcept
{
    return const_reverse_iterator(end());
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::const_reverse_iterator
map<Key, T, Compare, Allocator>::crend() const noexcept
{
    return rend();
}

template <typename Key, typename T, typename Compare, typename Allocator>
bool
map<Key, T, Compare, Allocator>::empty() const noexcept
{
    return begin() == end();
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::size_type
map<Key, T, Compare, Allocator>::size() const noexcept
{
    return distance(begin(), end());
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::size_type
map<Key, T, Compare, Allocator>::max_size() const noexcept
{
    return std::numeric_limits<size_type>::max();
}

template <typename Key, typename T, typename Compare, typename Allocator>
T&
map<Key, T, Compare, Allocator>::operator [](const key_type& _key)
{
    pair<iterator, bool> ret_pair = emplace(value_type(_key, mapped_type()));
    return (*(ret_pair.first)).second;
}

template <typename Key, typename T, typename Compare, typename Allocator>
T&
map<Key, T, Compare, Allocator>::operator [](key_type&& _key)
{
    pair<iterator, bool> ret_pair = emplace(value_type(std::move(_key),
                                            mapped_type()));
    return (*(ret_pair.first)).second;
}

template <typename Key, typename T, typename Compare, typename Allocator>
template <typename... Args>
pair<typename map<Key, T, Compare, Allocator>::iterator, bool>
map<Key, T, Compare, Allocator>::emplace(Args&& ..._args)
{
    pair<tree_iterator, bool> ret = p_tree_impl->emplace(
                                        saber::forward<Args>(_args)...);
    return pair<iterator, bool>(iterator(ret.first, this), ret.second);
}

template <typename Key, typename T, typename Compare, typename Allocator>
template <typename... Args>
typename map<Key, T, Compare, Allocator>::iterator
map<Key, T, Compare, Allocator>::emplace_hint(const_iterator, Args&& ..._args)
{
    return emplace(saber::forward<Args>(_args)...).first;
}

template <typename Key, typename T, typename Compare, typename Allocator>
pair<typename map<Key, T, Compare, Allocator>::iterator, bool>
map<Key, T, Compare, Allocator>::insert(const value_type &_pair)
{
    return emplace(_pair);
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::iterator
map<Key, T, Compare, Allocator>::insert(const_iterator, const value_type &_pair)
{
    return insert(_pair).first;
}



template <typename Key, typename T, typename Compare, typename Allocator>
map<Key, T, Compare, Allocator>::iterator::iterator(
        const map::const_iterator& _const_iter) :
    iterator(_const_iter.actual_iter, _const_iter.get_from)
{}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::iterator::reference
map<Key, T, Compare, Allocator>::iterator::operator* ()
{
    return *actual_iter;
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::iterator::const_reference
map<Key, T, Compare, Allocator>::iterator::operator* () const
{
    return *actual_iter;
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::iterator&
map<Key, T, Compare, Allocator>::iterator::operator++()
{
    ++actual_iter;
    return *this;
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::iterator&
map<Key, T, Compare, Allocator>::iterator::operator--()
{
    --actual_iter;
    return *this;
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::iterator
map<Key, T, Compare, Allocator>::iterator::operator++(int)
{
    iterator ret = *this;
    operator++();
    return ret;
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::iterator
map<Key, T, Compare, Allocator>::iterator::operator--(int)
{
    iterator ret = *this;
    operator--();
    return ret;
}

template <typename Key, typename T, typename Compare, typename Allocator>
bool
map<Key, T, Compare, Allocator>::iterator::operator== (
        const iterator& _another) const
{
    return actual_iter == _another.actual_iter;
}

template <typename Key, typename T, typename Compare, typename Allocator>
bool
map<Key, T, Compare, Allocator>::iterator::operator!= (
        const iterator& _another) const
{
    return !operator== (_another);
}



template <typename Key, typename T, typename Compare, typename Allocator>
map<Key, T, Compare, Allocator>::const_iterator::const_iterator(
        const map::iterator& _mutable_iter) :
    const_iterator(_mutable_iter.actual_iter, _mutable_iter.get_from)
{}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::const_iterator::const_reference
map<Key, T, Compare, Allocator>::const_iterator::operator* () const
{
    return *actual_iter;
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::const_iterator&
map<Key, T, Compare, Allocator>::const_iterator::operator++()
{
    ++actual_iter;
    return *this;
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::const_iterator&
map<Key, T, Compare, Allocator>::const_iterator::operator--()
{
    --actual_iter;
    return *this;
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::const_iterator
map<Key, T, Compare, Allocator>::const_iterator::operator++(int)
{
    const_iterator ret = *this;
    operator++();
    return ret;
}

template <typename Key, typename T, typename Compare, typename Allocator>
typename map<Key, T, Compare, Allocator>::const_iterator
map<Key, T, Compare, Allocator>::const_iterator::operator--(int)
{
    const_iterator ret = *this;
    operator--();
    return ret;
}

template <typename Key, typename T, typename Compare, typename Allocator>
bool
map<Key, T, Compare, Allocator>::const_iterator::operator== (
        const const_iterator& _another) const
{
    return actual_iter == _another.actual_iter;
}

template <typename Key, typename T, typename Compare, typename Allocator>
bool
map<Key, T, Compare, Allocator>::const_iterator::operator!= (
        const const_iterator& _another) const
{
    return !operator==(_another);
}

} // namespace saber

#endif // MAP_HPP
