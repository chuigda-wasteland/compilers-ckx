#ifdef _MSC_VER
#pragma once
#endif

#ifndef FORWARD_LIST_HPP
#define FORWARD_LIST_HPP

#include "algorithm.hpp"
#include "iterator.hpp"
#include "memory.hpp"
#include "saber_traits.hpp"

#include <unordered_set>

namespace saber
{

template <typename T, typename Allocator = default_allocator<T>>
class forward_list
{
public:
    using value_type       = T;
    using pointer          = T*;
    using reference        = T&;
    using const_reference  = const T&;
    using size_type        = size_t;
    using difference_type  = std::ptrdiff_t;

    class iterator;
    class const_iterator;

    using reverse_iterator = saber::reverse_iterator<iterator>;
    using allocator_type = Allocator;

    explicit forward_list();
    explicit forward_list(const Allocator& _alloc);
    forward_list(const forward_list& _another);
    forward_list(const forward_list& _another,
                 const Allocator& _alloc);
    explicit forward_list(size_type _n,
                          const Allocator& _alloc = Allocator());
    forward_list(size_type _n, const value_type& _value,
                 const Allocator& _alloc = Allocator());
    template <typename InputIterator>
    forward_list(InputIterator _first, InputIterator _last,
                 const Allocator& _alloc = Allocator());
    forward_list(std::initializer_list<value_type> _ilist,
                 const Allocator& _alloc = Allocator());

    ~forward_list();

    template <typename InputIterator>
    void assign(InputIterator _first, InputIterator _last);
    void assign(size_type _n, const value_type& _value);

    reference front();
    const_reference front() const;

    size_type size() const;
    size_type max_size() const;
    bool empty() const;     

    void clear();

    template <typename... Args>
    iterator emplace_after(const_iterator _pos, Args&&... _args);

    iterator insert_after(const_iterator _pos, const value_type& _value);
    iterator insert_after(const_iterator _pos, value_type&& _value);
    iterator insert_after(const_iterator _pos,
                          size_type _n, const value_type &_value);
    template <typename InputIterator>
    iterator insert_after(const_iterator _pos,
                          InputIterator _first, InputIterator _last);
    iterator insert_after(const_iterator _pos,
                          initializer_list<value_type> _ilist);

    iterator erase_after(const_iterator _pos);
    iterator erase_after(const_iterator _first, const_iterator _last);

    void splice_after(const_iterator _pos, forward_list& _from);
    void splice_after(const_iterator _pos, forward_list&& _from);
    void splice_after(const_iterator _pos, forward_list& _from,
                      const_iterator _before_first);
    void splice_after(const_iterator _pos, forward_list&& _from,
                      const_iterator _before_first);
    void splice_after(const_iterator _pos, forward_list& _from,
                      const_iterator _before_first,
                      const_iterator _last);
    void splice_after(const_iterator _pos, forward_list&& _from,
                      const_iterator _before_first,
                      const_iterator _last);

    void unique();
    template <typename BinaryPredicate>
    void unique(BinaryPredicate _pred);

    void remove(const value_type& _value);
    template <typename Predicate>
    void remove_if(Predicate _pred);

    template <typename Compare>
    void merge(forward_list& _another, Compare _comp);
    template <typename Compare>
    void merge(forward_list&& _another, Compare _comp);

    void reverse() noexcept;

    iterator begin() noexcept;
    iterator end() noexcept;

    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;

    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    iterator before_begin() noexcept;
    const_iterator before_begin() const noexcept;
    const_iterator cbefore_begin() const noexcept;

private:
    struct list_node_base
    {
        list_node_base *next = nullptr;

        list_node_base() = default;
        ~list_node_base() = default;
    };

    struct list_node : public list_node_base
    {
        T value;
        template <typename... Args>
        list_node(Args&& ..._args) : value(_args...) {}
        ~list_node() = default;
    };

    void register_node(const list_node_base *_node);
    void detach_node(const list_node_base *_node);
    bool search_node(const list_node_base *_node) const;

    template <typename ForwardListIterator>
    void check_iterator(const ForwardListIterator& _iter) const noexcept;

    using node_allocator_type = 
        typename Allocator::template rebind<list_node>::other;

    list_node_base *head;
    list_node_base *tail;
    allocator_type alloc;
    node_allocator_type node_alloc;

    saber_ptr<bool> validating_ptr;
    std::unordered_set<const list_node_base*> nodes;
};

template <typename T, typename Allocator>
class forward_list<T, Allocator>::iterator
{
    friend class forward_list;
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type        = typename forward_list::value_type;
    using difference_type   = typename forward_list::difference_type;
    using pointer           = typename forward_list::pointer;
    using reference         = typename forward_list::reference;
    using const_reference   = typename forward_list::const_reference;

    iterator(void) = default;
    iterator(const iterator& _another) = default;
    // See C++ Defect Report #179
    iterator(const typename forward_list::const_iterator& _const_iterator);
    ~iterator() = default;

    reference operator* (void);
    const_reference operator* (void) const;

    bool operator== (const iterator& _another) const;
    bool operator!= (const iterator& _another) const;

    iterator& operator++ (void);
    iterator operator++ (int);

private:
    iterator(forward_list *_get_from,
             typename forward_list::list_node_base *_node);
    void check_initialized(void) const noexcept;
    void check_valid(void) const noexcept;
    void check_dereferencable(void) const noexcept;

    forward_list *get_from = nullptr;
    typename forward_list::list_node_base *node = nullptr;
    saber_ptr<bool> validating_ptr; 
};

template <typename T, typename Allocator>
class forward_list<T, Allocator>::const_iterator
{
    friend class forward_list;
public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = typename forward_list::value_type;
    using difference_type = typename forward_list::difference_type;
    using pointer = typename forward_list::pointer;
    using reference = typename forward_list::reference;
    using const_reference = typename forward_list::const_reference;

    const_iterator() = default;
    const_iterator(const const_iterator& _another) = default;
    // See C++ Defect Report #179
    const_iterator(const typename forward_list::iterator& _mutable_iterator);
    ~const_iterator() = default;

    bool operator== (const const_iterator& _another) const;
    bool operator!= (const const_iterator& _another) const;

    const_reference operator* (void) const;
    const_iterator& operator++ (void);
    const_iterator operator++ (int);

private:
    const_iterator(const forward_list* _get_from,
                   const typename forward_list::list_node_base *_node);

    void check_initialized(void) const noexcept;
    void check_valid(void) const noexcept;
    void check_dereferencable(void) const noexcept;

    const forward_list *get_from = nullptr;
    const typename forward_list::list_node_base *node = nullptr;
    saber_ptr<bool> validating_ptr;
};

template <typename T, typename Allocator>
forward_list<T, Allocator>::forward_list() :
    forward_list(Allocator())
{
}

template <typename T, typename Allocator>
forward_list<T, Allocator>::forward_list(const Allocator& _alloc) :
    head(new list_node_base()),
    tail(new list_node_base()),
    alloc(_alloc),
    node_alloc(alloc),
    validating_ptr(new bool(true))
{
    head->next = tail;
    tail->next = head;
    register_node(head);
    register_node(tail);
}

template <typename T, typename Allocator>
forward_list<T, Allocator>::forward_list(const forward_list &_another) :
    forward_list(_another, _another.alloc)
{
}

template <typename T, typename Allocator>
forward_list<T, Allocator>::forward_list(const forward_list &_another,
                                         const Allocator &_alloc) :
    forward_list(_alloc)
{
    insert_after(cbefore_begin(), _another.begin(), _another.end());
}

template <typename T, typename Allocator>
forward_list<T, Allocator>::forward_list(size_type _n,
                                         const Allocator &_alloc) :
    forward_list(_n, value_type(), _alloc)
{
}

template <typename T, typename Allocator>
forward_list<T, Allocator>::forward_list(size_type _n,
                                         const value_type &_value,
                                         const Allocator &_alloc) :
    forward_list(_alloc)
{
    insert_after(cbefore_begin(), _n, _value);
}

template <typename T, typename Allocator>
template <typename InputIterator>
forward_list<T, Allocator>::forward_list(
        InputIterator _first, InputIterator _last,
        const Allocator& _alloc) :
    forward_list(_alloc)
{
    insert_after(cbefore_begin(), _first, _last);
}

template <typename T, typename Allocator>
forward_list<T, Allocator>::forward_list(
        std::initializer_list<value_type> _ilist, const Allocator &_alloc) :
    forward_list(_ilist.begin(), _ilist.end(), _alloc)
{
}

template <typename T, typename Allocator>
forward_list<T, Allocator>::~forward_list()
{
    erase_after(cbefore_begin(), cend());
    delete head;
    delete tail;
    *(validating_ptr.get()) = false;
}

template <typename T, typename Allocator>
template <typename InputIterator>
void
forward_list<T, Allocator>::assign(InputIterator _first, InputIterator _last)
{
    clear();
    insert_after(cbefore_begin(), _first, _last);
}

template <typename T, typename Allocator>
void
forward_list<T, Allocator>::assign(size_type _n, const value_type &_value)
{
    clear();
    insert_after(cbefore_begin(), size_type(_n), _value);
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::size_type
forward_list<T, Allocator>::size() const
{
    return distance(begin(), end());
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::size_type
forward_list<T, Allocator>::max_size() const
{
    return std::numeric_limits<size_type>::max();
}

template <typename T, typename Allocator>
bool
forward_list<T, Allocator>::empty() const
{
    return (begin() == end());
}

template <typename T, typename Allocator>
void
forward_list<T, Allocator>::clear()
{
    erase_after(cbefore_begin(), cend());
}

template <typename T, typename Allocator>
template <typename... Args>
typename forward_list<T, Allocator>::iterator
forward_list<T, Allocator>::emplace_after(const_iterator _pos, Args&&... _args)
{
    _pos.check_valid();
    check_iterator(_pos);

    list_node *node =
            allocator_traits<node_allocator_type>::allocate(node_alloc, 1);
    construct(node, saber::forward<Args>(_args)...);

    iterator mutable_iterator(_pos);
    node->next = mutable_iterator.node->next;
    mutable_iterator.node->next = node;

    register_node(node);
    return iterator(this, node);
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::iterator
forward_list<T, Allocator>::insert_after(const_iterator _pos,
                                         const value_type& _value)
{
    return emplace_after(_pos, _value);
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::iterator
forward_list<T, Allocator>::insert_after(const_iterator _pos,
                                         size_type _n,
                                         const value_type& _value)
{
    for (size_type i = 0; i < _n; ++i)
    {
        emplace_after(_pos, _value);
    }
    return iterator(_pos);
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::iterator
forward_list<T, Allocator>::insert_after(const_iterator _pos,
                                         value_type &&_value)
{
    return emplace_after(_pos, std::move(_value));
}

template <typename T, typename Allocator>
template <typename InputIterator>
typename forward_list<T, Allocator>::iterator
forward_list<T, Allocator>::insert_after(const_iterator _pos,
                                         InputIterator _first,
                                         InputIterator _last)
{
    for (; _first != _last; ++_first)
    {
        _pos = emplace_after(_pos, *_first);
    }

    return iterator(_pos);
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::iterator
forward_list<T, Allocator>::insert_after(const_iterator _pos,
                                         initializer_list<value_type> _ilist)
{
    return insert_after(_pos, _ilist.begin(), _ilist.end());
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::iterator
forward_list<T, Allocator>::erase_after(const_iterator _pos)
{
    _pos.check_valid();
    check_iterator(_pos);

    iterator mutable_iterator(_pos);

    list_node_base *deleted_node = _pos.node->next;
    mutable_iterator.node->next = deleted_node->next;

    destroy_at(deleted_node);
    detach_node(deleted_node);
    allocator_traits<node_allocator_type>::deallocate(
                node_alloc,
                reinterpret_cast<typename forward_list::list_node*>
                    (deleted_node),
                1);

    return mutable_iterator;
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::iterator
forward_list<T, Allocator>::erase_after(const_iterator _first,
                                        const_iterator _last)
{
    for (; _first.node->next != _last.node; erase_after(_first));
    return iterator(_first);
}

template <typename T, typename Allocator>
void
forward_list<T, Allocator>::splice_after(const_iterator _pos,
                                         forward_list &_from)
{
    splice_after(_pos, _from, _from.cbefore_begin(), _from.cend());
}

template <typename T, typename Allocator>
void
forward_list<T, Allocator>::splice_after(const_iterator _pos,
                                         forward_list &_from,
                                         const_iterator _before_first)
{
    splice_after(_pos, _from, _before_first, _from.cend());
}

template <typename T, typename Allocator>
void
forward_list<T, Allocator>::splice_after(const_iterator _pos,
                                         forward_list& _from,
                                         const_iterator _before_first,
                                         const_iterator _last)
{
    if (alloc == _from.alloc)
    {
        _pos.check_valid();
        check_iterator(_pos);
        _before_first.check_valid();
        _last.check_valid();
        _from.check_iterator(_before_first);
        _from.check_iterator(_last);

        list_node_base *pos_node =
                const_cast<list_node_base*>(_pos.node);
        list_node_base *before_first_node =
                const_cast<list_node_base*>(_before_first.node);
        list_node_base *last_node =
                const_cast<list_node_base*>(_last.node);

        for (list_node_base *node_it = before_first_node->next;
             node_it != last_node;
             node_it = node_it->next)
        {
            register_node(node_it);
            _from.detach_node(node_it);
        }

        {
            list_node_base *before_last_node;
            for (before_last_node = before_first_node;
                 before_last_node->next != last_node;
                 before_last_node = before_last_node->next);

            before_last_node->next = pos_node->next;
            pos_node->next = before_first_node->next;
            before_first_node->next = last_node;
        }
    }
    else
    {
        stl_warning(C8_DYN__CONT__SPLICE_BETWEEN_UNEQUAL_ALLOC_CONTAINERS);
        insert_after(_pos, ++_before_first, _last);
        _from.erase_after(_before_first, _last);
    }
}

template <typename T, typename Allocator>
void
forward_list<T, Allocator>::unique()
{
    unique(std::equal_to<T>());
}

template <typename T, typename Allocator>
template <typename BinaryPredicate>
void
forward_list<T, Allocator>::unique(BinaryPredicate _binary_pred)
{
    if (cbegin() == cend()) return;

    auto it = cbegin(),
         it2 = ++cbegin();
    while (it2 != cend())
    {
        if (_binary_pred(*it, *it2))
        {
            it = erase_after(it);
        }
        else
        {
            ++it;
        }

        it2 = it++;
        saber::swap(it2, it);
    }
}

template <typename T, typename Allocator>
void
forward_list<T, Allocator>::remove(const value_type &_value)
{
    remove_if(
                [&](const value_type& _another_value) -> bool
                {
                    return _value == _another_value;
                }
    );
}

template <typename T, typename Allocator>
template <typename Predicate>
void
forward_list<T, Allocator>::remove_if(Predicate _pred)
{
    if (cbegin() == cend()) return;

    auto it = cbefore_begin(),
         it2 = cbegin();
    while (it2 != cend())
    {
        if (_pred(*it2))
        {
            it = erase_after(it);
        }
        else
        {
            it++;
        }

        it2 = it++;
        saber::swap(it2, it);
    }
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::iterator
forward_list<T, Allocator>::begin() noexcept
{
    return iterator(this, head->next);
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::iterator
forward_list<T, Allocator>::end() noexcept
{
    return iterator(this, tail);
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::const_iterator
forward_list<T, Allocator>::begin() const noexcept
{
    return const_iterator(this, head->next);
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::const_iterator
forward_list<T, Allocator>::end() const noexcept
{
    return const_iterator(this, tail);
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::const_iterator
forward_list<T, Allocator>::cbegin() const noexcept
{
    return begin();
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::const_iterator
forward_list<T, Allocator>::cend() const noexcept
{
    return end();
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::iterator
forward_list<T, Allocator>::before_begin() noexcept
{
    return iterator(this, head);
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::const_iterator
forward_list<T, Allocator>::before_begin() const noexcept
{
    return const_iterator(this, head);
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::const_iterator
forward_list<T, Allocator>::cbefore_begin() const noexcept
{
    return before_begin();
}

template <typename T, typename Allocator>
void
forward_list<T, Allocator>::register_node(const list_node_base *_node)
{
    nodes.insert(_node);
}

template <typename T, typename Allocator>
void
forward_list<T, Allocator>::detach_node(const list_node_base *_node)
{
    nodes.erase(_node);
}

template <typename T, typename Allocator>
bool
forward_list<T, Allocator>::search_node(const list_node_base *_node) const
{
    return (nodes.find(_node) != nodes.end());
}

template <typename T, typename Allocator>
template <typename ForwardListIterator>
void
forward_list<T, Allocator>::check_iterator(
        const ForwardListIterator& _iter) const noexcept
{
    if (!search_node(_iter.node))
    {
        stl_panic(C8_DYN__ITER__UNKNOWN_REGION_ITERATOR);
    }
}



template <typename T, typename Allocator>
forward_list<T, Allocator>::iterator::iterator(
        const typename forward_list::const_iterator& _const_iterator) :
    get_from(const_cast<forward_list*>(_const_iterator.get_from)),
    node(const_cast<typename forward_list::list_node_base*>
         (_const_iterator.node)),
    validating_ptr(_const_iterator.validating_ptr)
{
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::iterator::reference
forward_list<T, Allocator>::iterator::operator* ()
{
    check_dereferencable();
    return reinterpret_cast<typename forward_list::list_node*>(node)->value;
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::iterator::const_reference
forward_list<T, Allocator>::iterator::operator* () const
{
    check_dereferencable();
    return reinterpret_cast<typename forward_list::list_node*>(node)->value;
}

template <typename T, typename Allocator>
bool
forward_list<T, Allocator>::iterator::operator== (
        const iterator& _another) const
{
    check_valid();
    return (node == _another.node);
}

template <typename T, typename Allocator>
bool
forward_list<T, Allocator>::iterator::operator!= (
        const iterator& _another) const
{
    return !operator== (_another);
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::iterator&
forward_list<T, Allocator>::iterator::operator++ ()
{
    check_valid();
    node = node->next;
    return *this;
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::iterator
forward_list<T, Allocator>::iterator::operator++(int)
{
    check_valid();
    iterator temp(*this);
    node = node->next;
    return temp;
}

template <typename T, typename Allocator>
forward_list<T, Allocator>::iterator::iterator(
        forward_list *_get_from,
        typename forward_list::list_node_base *_node) :
    get_from(_get_from),
    node(_node),
    validating_ptr(_get_from->validating_ptr)
{
}

template <typename T, typename Allocator>
void
forward_list<T, Allocator>::iterator::check_initialized() const noexcept
{
    if (get_from == nullptr)
    {
        assert(node == nullptr);
        assert(validating_ptr.get() == nullptr);
        stl_panic(C8_DYN__ITER__UNINITIALIZED_ITERATOR);
   }
}

template <typename T, typename Allocator>
void
forward_list<T, Allocator>::iterator::check_valid() const noexcept
{
    check_initialized();

    if (*(validating_ptr.get()) == false)
    {
        stl_panic(C8_DYN__ITER__DELETED_CONTAINER);
    }

    if (! get_from->search_node(node))
    {
        stl_panic(C8_DYN__ITER__OLD_ITERATOR);
    }
}

template <typename T, typename Allocator>
void
forward_list<T, Allocator>::iterator::check_dereferencable() const noexcept
{
    check_valid();
    if (node == get_from->head)
    {
        stl_panic(C8_DYN__ITER__ITERATOR_OVERFLOW);
    }
}



template <typename T, typename Allocator>
forward_list<T, Allocator>::const_iterator::const_iterator(
        const typename forward_list::iterator& _mutable_iterator) :
    get_from(_mutable_iterator.get_from),
    node(_mutable_iterator.node),
    validating_ptr(_mutable_iterator.validating_ptr)
{
}

template <typename T, typename Allocator>
bool
forward_list<T, Allocator>::const_iterator::operator== (
        const const_iterator& _another) const
{
    check_valid();
    return (node == _another.node);
}

template <typename T, typename Allocator>
bool
forward_list<T, Allocator>::const_iterator::operator!= (
        const const_iterator& _another) const
{
    check_valid();
    return ! operator==(_another);
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::const_iterator::const_reference
forward_list<T, Allocator>::const_iterator::operator*() const
{
    check_dereferencable();
    return reinterpret_cast<const forward_list::list_node*>(node)->value;
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::const_iterator&
forward_list<T, Allocator>::const_iterator::operator++ ()
{
    check_valid();
    node = node->next;
    return *this;
}

template <typename T, typename Allocator>
typename forward_list<T, Allocator>::const_iterator
forward_list<T, Allocator>::const_iterator::operator++ (int)
{
    check_valid();
    const_iterator temp(*this);
    node = node->next;
    return temp;
}

template <typename T, typename Allocator>
forward_list<T, Allocator>::const_iterator::const_iterator(
        const forward_list* _get_from,
        const typename forward_list::list_node_base *_node) :
    get_from(_get_from),
    node(_node),
    validating_ptr(_get_from->validating_ptr)
{
}

template <typename T, typename Allocator>
void
forward_list<T, Allocator>::const_iterator::check_initialized() const noexcept
{
    if (get_from == nullptr)
    {
        assert(node == nullptr);
        assert(validating_ptr.get() == nullptr);
        stl_panic(C8_DYN__ITER__UNINITIALIZED_ITERATOR);
    }
}

template <typename T, typename Allocator>
void
forward_list<T, Allocator>::const_iterator::check_valid() const noexcept
{
    check_initialized();

    if (*(validating_ptr.get()) == false)
    {
        stl_panic(C8_DYN__ITER__DELETED_CONTAINER);
    }

    if (!get_from->search_node(node))
    {
        stl_panic(C8_DYN__ITER__OLD_ITERATOR);
    }
}

template <typename T, typename Allocator>
void
forward_list<T, Allocator>::const_iterator::check_dereferencable() const noexcept
{
    check_valid();

    if (node == get_from->head)
    {
        stl_panic(C8_DYN__ITER__ITERATOR_OVERFLOW);
    }

}

} // namespace saber

#endif // FORWARD_LIST_HPP

