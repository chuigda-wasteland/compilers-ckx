//////////////////////////////////////////////////////////////////////////////

//  芙蓉城三月雨纷纷 四月绣花针                                             //
//  羽毛扇遥指千军阵 锦缎裁几寸                                             //
//  看铁马踏冰河 丝线缝韶华 红尘千帐灯                                      //
//  山水一程风雪再一程                                                      //
//  红烛枕五月花叶深 六月杏花村                                             //
//  红酥手青丝万千根 姻缘多一分                                             //
//  等残阳照孤影 牡丹染铜樽 满城牧笛声                                      //
//  伊人倚门望君踏归程                                                      //

//  君可见刺绣每一针 有人为你疼                                             //
//  君可见牡丹开一生 有人为你等                                             //
//  江河入海奔 万物为谁春                                                   //
//  明月照不尽离别人                                                        //
//  君可见刺绣又一针 有人为你疼                                             //
//  君可见夏雨秋风 有人为你等                                               //
//  翠竹泣墨痕 锦书画不成                                                   //
//  情针意线绣不尽 鸳鸯枕                                                   //

//  此生笑傲风月瘦如刀 催人老                                               //
//  来世与君暮暮又朝朝 多逍遥                                               //

//  绕指柔破锦千万针 杜鹃啼血声                                             //
//  芙蓉花蜀国尽缤纷 转眼尘归尘                                             //
//  战歌送离人 行人欲断魂                                                   //
//  浓情蜜意此话当真

//  君可见刺绣每一针 有人为你疼                                             //
//  君可见牡丹开一生 有人为你等                                             //
//  江河入海奔 万物为谁春                                                   //
//  明月照不尽离别人                                                        //
//  君可见刺绣又一针 有人为你疼                                             //
//  君可见夏雨秋风有人 为你等                                               //
//  翠竹泣墨痕 锦书画不成                                                   //
//  情针意线绣不尽 鸳鸯枕                                                   //

//////////////////////////////////////////////////////////////////////////////


#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIST_HPP
#define LIST_HPP

#include "algorithm.hpp"
#include "iterator.hpp"
#include "memory.hpp"
#include "saber_traits.hpp"

#include <unordered_set>

namespace saber
{

template <typename T, typename Allocator = default_allocator<T>>
class list
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
    using const_reverse_iterator = saber::reverse_iterator<const_iterator>;
    using allocator_type = Allocator;

    explicit list();
    explicit list(const Allocator& _alloc);
    list(const list& _another);
    list(const list& _another, const Allocator& _alloc);
    list(size_type _n, const Allocator& _alloc = Allocator());
    list(size_type _n, const value_type& _value,
         const Allocator& _alloc = Allocator());
    template <typename InputIterator>
    list(InputIterator _first, InputIterator _last,
         const Allocator& _alloc = Allocator());
    list(std::initializer_list<value_type> _list,
         const Allocator& _alloc = Allocator());
    ~list();

    size_type size() const;
    bool empty() const;

    iterator insert(const_iterator _position, const value_type& _value);
    iterator insert(const_iterator _position, value_type&& _value);
    iterator insert(const_iterator _position,
                    size_type _n, const value_type &_value);
    template <typename InputIterator>
    iterator insert(const_iterator _position,
                    InputIterator _first, InputIterator _last);
    iterator insert(const_iterator _position,
                    initializer_list<value_type> _ilist);

    template <typename... Args>
    iterator emplace(const_iterator _position, Args&& ..._args);
    template <typename... Args>
    iterator emplace_back(Args&& ..._args);
    template <typename... Args>
    iterator emplace_front(Args&& ..._args);

    void push_back(const value_type& _value);
    void push_front(const value_type& _value);

    iterator erase(const_iterator _position);
    iterator erase(const_iterator _first, const_iterator _last);

    void pop_back();
    void pop_front();

    void clear() noexcept;

    void remove(const value_type& _value);
    template <typename UnaryPredicate>
    void remove_if(UnaryPredicate _pred);

    void unique();
    template <typename BinaryPredicate>
    void unique(BinaryPredicate _pred);

    void splice(const_iterator _pos, list& _other);
    void splice(const_iterator _pos, list&& _other);
    void splice(const_iterator _pos, list& _other, const_iterator _it);
    void splice(const_iterator _pos, list&& _other, const_iterator _it);
    void splice(const_iterator _pos, list& _other,
                const_iterator _first, const_iterator _last);
    void splice(const_iterator _pos, list&& _other,
                const_iterator _first, const_iterator _last);

    void swap(list& _another) noexcept;

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
    struct list_node_base
    {
        list_node_base* prev = nullptr;
        list_node_base* next = nullptr;

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

    void register_node(const list_node_base* _node);
    void detach_node(const list_node_base* _node);
    bool search_node(const list_node_base* _node) const;

    template <typename ListIterator>
    void check_iterator(const ListIterator& _iter) const noexcept;

    using node_allocator_type =
        typename Allocator::template rebind<list_node>::other;

    list_node_base *head;

    allocator_type alloc;
    node_allocator_type node_alloc;

    saber_ptr<bool> validating_ptr;
    std::unordered_set<const list_node_base*> nodes;
};

template <typename T, typename Allocator>
class list<T, Allocator>::iterator
{
    friend class list;
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = typename list::value_type;
    using difference_type   = typename list::difference_type;
    using pointer           = typename list::pointer;
    using reference         = typename list::reference;
    using const_reference   = typename list::const_reference;

    iterator(void) = default;
    iterator(const iterator& _another) = default;
    // See C++ Defect Report #179
    iterator(const typename list::const_iterator& _const_iterator);
    ~iterator() = default;

    reference operator* (void);
    const_reference operator* (void) const;

    bool operator== (const iterator& _another) const;
    bool operator!= (const iterator& _another) const;

    iterator& operator++ (void);
    iterator& operator-- (void);
    iterator operator++ (int);
    iterator operator-- (int);

private:
    iterator(list *_get_from, typename list::list_node_base *_node);

    void check_initialized(void) const noexcept;
    void check_valid(void) const noexcept;
    void check_dereferencable(void) const noexcept;

    list *get_from = nullptr;
    typename list::list_node_base *node = nullptr;
    saber_ptr<bool> validating_ptr;
};

template <typename T, typename Allocator>
class list<T, Allocator>::const_iterator
{
    friend class list;
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = typename list::value_type;
    using difference_type   = typename list::difference_type;
    using pointer           = typename list::pointer;
    using reference         = typename list::reference;
    using const_reference   = typename list::const_reference;

    const_iterator() = default;
    const_iterator(const const_iterator& _another) = default;
    // See C++ Defect Report #179
    const_iterator(const typename list::iterator& _mutable_iterator);
    ~const_iterator() = default;

    bool operator== (const const_iterator& _another) const;
    bool operator!= (const const_iterator& _another) const;

    const_reference operator* (void) const;
    const_iterator& operator++ (void);
    const_iterator& operator-- (void);
    const_iterator operator++ (int);
    const_iterator operator-- (int);

private:
    const_iterator(const list *_get_from, 
                   const typename list::list_node_base *_node);

    void check_initialized(void) const noexcept;
    void check_valid(void) const noexcept;
    void check_dereferencable(void) const noexcept;

    const list *get_from = nullptr;
    const typename list::list_node_base *node = nullptr;
    saber_ptr<bool> validating_ptr;
};

template <typename T, typename Allocator>
list<T, Allocator>::list() :
    list(Allocator())
{
}

template <typename T, typename Allocator>
list<T, Allocator>::list(const Allocator &_alloc) :
    head(new list_node_base()),
    alloc(_alloc),
    node_alloc(alloc),
    validating_ptr(new bool(true))
{
    head->prev = head;
    head->next = head;

    register_node(head);
}

template <typename T, typename Allocator>
list<T, Allocator>::list(const list& _another) :
    list(_another, _another.alloc)
{
}

template <typename T, typename Allocator>
list<T, Allocator>::list(const list &_another, const Allocator &_alloc) :
    list(_alloc)
{
    insert(cend(), _another.cbegin(), _another.cend());
}

template <typename T, typename Allocator>
list<T, Allocator>::list(size_type _n,
                         const value_type &_value,
                         const Allocator &_alloc) :
    list(_alloc)
{
    insert(cend(), _n, _value);
}

template <typename T, typename Allocator>
list<T, Allocator>::list(size_type _n, const Allocator &_alloc) :
    list(_n, value_type(), _alloc)
{
}

template <typename T, typename Allocator>
list<T, Allocator>::list(initializer_list<value_type> _ilist,
                         const Allocator& _alloc) :
    list(_alloc)
{
    insert(cend(), _ilist.begin(), _ilist.end());
}

template <typename T, typename Allocator>
template <typename InputIterator>
list<T, Allocator>::list(InputIterator _first, InputIterator _last,
                         const Allocator& _alloc) :
    list(_alloc)
{
    insert(cend(), _first, _last);
}

template <typename T, typename Allocator>
list<T, Allocator>::~list()
{
    erase(cbegin(), cend());
    delete head;
    *(validating_ptr.get()) = false;
}

template <typename T, typename Allocator>
typename list<T, Allocator>::size_type
list<T, Allocator>::size() const
{
    return distance(begin(), end());
}

template <typename T, typename Allocator>
bool
list<T, Allocator>::empty() const
{
    return (head->next == head);
}

template <typename T, typename Allocator>
typename list<T, Allocator>::iterator
list<T, Allocator>::insert(const_iterator _position, const value_type &_value)
{
    return emplace(_position, _value);
}

template <typename T, typename Allocator>
typename list<T, Allocator>::iterator
list<T, Allocator>::insert(const_iterator _position, value_type &&_value)
{
    return emplace(_position, std::move(_value));
}

template <typename T, typename Allocator>
typename list<T, Allocator>::iterator
list<T, Allocator>::insert(const_iterator _position,
                           size_type _n, const value_type &_value)
{
    for (size_type i = 0; i < _n; ++i)
    {
        insert(_position, _value);
    }
}

template <typename T, typename Allocator>
template <typename InputIterator>
typename list<T, Allocator>::iterator
list<T, Allocator>::insert(const_iterator _position,
                           InputIterator _first, InputIterator _last)
{
    for (; _first != _last; ++_first)
    {
        insert(_position, *_first);
    }

    return iterator(this, const_cast<list_node_base*>(_position.node));
}

template <typename T, typename Allocator>
typename list<T, Allocator>::iterator
list<T, Allocator>::insert(const_iterator _position,
                           initializer_list<value_type> _ilist)
{
    return insert(_position, _ilist.begin(), _ilist.end());
}

template <typename T, typename Allocator>
template <typename... Args>
typename list<T, Allocator>::iterator
list<T, Allocator>::emplace(const_iterator _position, Args&&... _args)
{
    _position.check_valid();
    check_iterator(_position);
    iterator position(this, const_cast<list_node_base*>(_position.node));

    list_node *node =
            allocator_traits<node_allocator_type>::allocate(node_alloc, 1);
    construct(node, saber::forward<Args>(_args)...);

    node->prev = position.node->prev;
    node->next = position.node;

    position.node->prev->next = node;
    position.node->prev = node;

    register_node(node);

    return iterator(this, node);
}

template <typename T, typename Allocator>
template <typename... Args>
typename list<T, Allocator>::iterator
list<T, Allocator>::emplace_back(Args&&... _args)
{
    emplace(cend(), saber::forward<Args>(_args)...);
}

template <typename T, typename Allocator>
template <typename... Args>
typename list<T, Allocator>::iterator
list<T, Allocator>::emplace_front(Args&&... _args)
{
    emplace(cbegin(), saber::forward<Args>(_args)...);
}

template <typename T, typename Allocator>
void
list<T, Allocator>::push_back(const value_type &_value)
{
    insert(cend(), _value);
}

template <typename T, typename Allocator>
void
list<T, Allocator>::push_front(const value_type &_value)
{
    insert(cbegin(), _value);
}

template <typename T, typename Allocator>
typename list<T, Allocator>::iterator
list<T, Allocator>::erase(const_iterator _position)
{
    _position.check_dereferencable();
    check_iterator(_position);
    list_node_base *prev = _position.node->prev,
                   *next = _position.node->next;

    prev->next = next;
    next->prev = prev;

    list_node_base *mutable_node =
            const_cast<list_node_base*>(_position.node);
    list_node *converted_node =
            reinterpret_cast<list_node*>(mutable_node);

    detach_node(_position.node);
    destroy_at(converted_node);
    allocator_traits<node_allocator_type>::deallocate(node_alloc,
                                                      converted_node, 1);
    return iterator(this, next);
}

template <typename T, typename Allocator>
typename list<T, Allocator>::iterator
list<T, Allocator>::erase(const_iterator _first, const_iterator _last)
{
    for  (; _first != _last; _first = erase(_first));
    return iterator(this, const_cast<list_node_base*>(_last.node));
}

template <typename T, typename Allocator>
void
list<T, Allocator>::pop_back()
{
    erase(--cend());
}

template <typename T, typename Allocator>
void
list<T, Allocator>::pop_front()
{
    erase(cbegin());
}

template <typename T, typename Allocator>
void
list<T, Allocator>::clear() noexcept
{
    erase(cbegin(), cend());
}

template <typename T, typename Allocator>
void
list<T, Allocator>::remove(const value_type &_value)
{
    remove_if(
        [&](const value_type& _another) -> bool
        {
            return _another == _value;
        }
    );
}

template <typename T, typename Allocator>
template <typename UnaryPredicate>
void
list<T, Allocator>::remove_if(UnaryPredicate _pred)
{
    for (auto it = begin(); it != end();)
    {
        if (_pred(*it))
        {
            it = erase(it);
        }
        else
        {
            ++it;
        }
    }
}

template <typename T, typename Allocator>
void
list<T, Allocator>::unique()
{
    unique(std::equal_to<T>());
}

template <typename T, typename Allocator>
template <typename BinaryPredicate>
void
list<T, Allocator>::unique(BinaryPredicate _binary_pred)
{
    if (cbegin() == cend()) return;

    auto it = cbegin(),
         it2 = ++cbegin();
    while (it2 != cend())
    {
        if (_binary_pred(*it, *it2))
        {
            it = erase(it);
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
list<T, Allocator>::splice(const_iterator _pos, list &_other)
{
    splice(_pos, _other, _other.begin(), _other.end());
}

template <typename T, typename Allocator>
void
list<T, Allocator>::splice(const_iterator _pos, list &_other,
                           const_iterator _it)
{
    splice(_pos, _other, _it, _other.cend());
}

template <typename T, typename Allocator>
void
list<T, Allocator>::splice(const_iterator _pos, list &_other,
                           const_iterator _first, const_iterator _last)
{
    _pos.check_valid();
    check_iterator(_pos);
    _first.check_valid();
    _last.check_valid();
    _other.check_iterator(_first);
    _other.check_iterator(_last);

    if (alloc == _other.alloc)
    {
        list_node_base *transfer_to =
                const_cast<list_node_base*>(_pos.node->prev);
        list_node_base *transfer_first =
                const_cast<list_node_base*>(_first.node);
        list_node_base *transfer_last =
                const_cast<list_node_base*>(_last.node->prev);

        for (auto first_blood = _first.node;
             first_blood != _last.node;
             first_blood = first_blood->next)
        {
            register_node(first_blood);
            _other.detach_node(first_blood);
        }

        transfer_first->prev->next = transfer_last->next;
        transfer_last->next->prev = transfer_first->prev;
        transfer_last->next = transfer_to->next;
        transfer_first->prev = transfer_to;
        transfer_to->next->prev = transfer_last;
        transfer_to->next = transfer_first;
    }
    else
    {
        stl_warning(C8_DYN__CONT__SPLICE_BETWEEN_UNEQUAL_ALLOC_CONTAINERS);
        insert(_pos, _first, _last);
        _other.erase(_first, _last);
    }
}

template <typename T, typename Allocator>
void
list<T, Allocator>::swap(list &_another) noexcept
{
    saber::swap(head, _another.head);
    saber::swap(alloc, _another.alloc);
    saber::swap(node_alloc, _another.node_alloc);
    saber::swap(nodes, _another.nodes);
}

template <typename T, typename Allocator>
typename list<T, Allocator>::iterator
list<T, Allocator>::begin() noexcept
{
    return iterator(this, head->next);
}

template <typename T, typename Allocator>
typename list<T, Allocator>::iterator
list<T, Allocator>::end() noexcept
{
    return iterator(this, head);
}

template <typename T, typename Allocator>
typename list<T, Allocator>::const_iterator
list<T, Allocator>::begin() const noexcept
{
    return const_iterator(this, head->next);
}

template <typename T, typename Allocator>
typename list<T, Allocator>::const_iterator
list<T, Allocator>::end() const noexcept
{
    return const_iterator(this, head);
}

template <typename T, typename Allocator>
typename list<T, Allocator>::const_iterator
list<T, Allocator>::cbegin() const noexcept
{
    return begin();
}

template <typename T, typename Allocator>
typename list<T, Allocator>::const_iterator
list<T, Allocator>::cend() const noexcept
{
    return end();
}

template <typename T, typename Allocator>
typename list<T, Allocator>::reverse_iterator
list<T, Allocator>::rbegin() noexcept
{
    return reverse_iterator(end());
}

template <typename T, typename Allocator>
typename list<T, Allocator>::reverse_iterator
list<T, Allocator>::rend() noexcept
{
    return reverse_iterator(begin());
}

template <typename T, typename Allocator>
typename list<T, Allocator>::const_reverse_iterator
list<T, Allocator>::rbegin() const noexcept
{
    return const_reverse_iterator(cend());
}

template <typename T, typename Allocator>
typename list<T, Allocator>::const_reverse_iterator
list<T, Allocator>::rend() const noexcept
{
    return const_reverse_iterator(cbegin());
}

template <typename T, typename Allocator>
typename list<T, Allocator>::const_reverse_iterator
list<T, Allocator>::crbegin() const noexcept
{
    return rbegin();
}

template <typename T, typename Allocator>
typename list<T, Allocator>::const_reverse_iterator
list<T, Allocator>::crend() const noexcept
{
    return rend();
}

template <typename T, typename Allocator>
void
list<T, Allocator>::register_node(const list_node_base *_node)
{
    nodes.insert(_node);
}

template <typename T, typename Allocator>
void
list<T, Allocator>::detach_node(const list_node_base *_node)
{
    nodes.erase(_node);
}

template <typename T, typename Allocator>
bool
list<T, Allocator>::search_node(const list_node_base *_node) const
{
    return (nodes.find(_node) != nodes.end());
}

template <typename T, typename Allocator>
template <typename ListIterator>
void
list<T, Allocator>::check_iterator(const ListIterator& _iter) const noexcept
{
    if (_iter.get_from != this)
    {
        stl_panic(C8_DYN__ITER__UNKNOWN_REGION_ITERATOR);
    }
}



template <typename T, typename Allocator>
list<T, Allocator>::iterator::iterator
    (const typename list::const_iterator& _const_iterator) :
    get_from(const_cast<list*>(_const_iterator.get_from)),
    node(const_cast<typename list::list_node_base*>(_const_iterator.node)),
    validating_ptr(_const_iterator.validating_ptr)
{
}

template <typename T, typename Allocator>
bool
list<T, Allocator>::iterator::operator== (const iterator& _another) const
{
    check_valid();
    return node == _another.node;
}

template <typename T, typename Allocator>
bool
list<T, Allocator>::iterator::operator!= (const iterator& _another) const
{
    return ! operator==(_another);
}

template <typename T, typename Allocator>
typename list<T, Allocator>::iterator::reference
list<T, Allocator>::iterator::operator*()
{
    check_dereferencable();
    return (reinterpret_cast<typename list::list_node*>(node))->value;
}

template <typename T, typename Allocator>
typename list<T, Allocator>::iterator::const_reference
list<T, Allocator>::iterator::operator*() const
{
    check_dereferencable();
    return (reinterpret_cast<const typename list::list_node*>(node))->value;
}

template <typename T, typename Allocator>
typename list<T, Allocator>::iterator&
list<T, Allocator>::iterator::operator++()
{
    check_valid();
    node = node->next;
    return *this;
}

template <typename T, typename Allocator>
typename list<T, Allocator>::iterator&
list<T, Allocator>::iterator::operator--()
{
    check_valid();
    node = node->prev;
    return *this;
}

template <typename T, typename Allocator>
typename list<T, Allocator>::iterator
list<T, Allocator>::iterator::operator++(int)
{
    check_valid();
    iterator ret(*this);
    node = node->next;
    return ret;
}

template <typename T, typename Allocator>
typename list<T, Allocator>::iterator
list<T, Allocator>::iterator::operator--(int)
{
    check_valid();
    iterator ret(*this);
    node = node->prev;
    return ret;
}

template <typename T, typename Allocator>
list<T, Allocator>::iterator::iterator(list *_get_from,
                                       typename list::list_node_base *_node) :
    get_from(_get_from),
    node(_node),
    validating_ptr(_get_from->validating_ptr)
{
}

template <typename T, typename Allocator>
void
list<T, Allocator>::iterator::check_initialized() const noexcept
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
list<T, Allocator>::iterator::check_valid() const noexcept
{
    if (*(validating_ptr.get()) == false)
    {
        stl_panic(C8_DYN__ITER__DELETED_CONTAINER);
    }

    if (get_from->search_node(node));
    else
    {
        stl_panic(C8_DYN__ITER__OLD_ITERATOR);
    }
}

template <typename T, typename Allocator>
void
list<T, Allocator>::iterator::check_dereferencable() const noexcept
{
    if (node == get_from->head)
    {
        stl_panic(C8_DYN__ITER__ITERATOR_OVERFLOW);
    }
}



template <typename T, typename Allocator>
list<T, Allocator>::const_iterator::const_iterator(
        const typename list::iterator& _mutable_iterator) :
    get_from(_mutable_iterator.get_from),
    node(_mutable_iterator.node),
    validating_ptr(_mutable_iterator.validating_ptr)
{
}

template <typename T, typename Allocator>
bool
list<T, Allocator>::const_iterator::operator== (
        const const_iterator& _another) const
{
    check_valid();
    return node == _another.node;
}

template <typename T, typename Allocator>
bool
list<T, Allocator>::const_iterator::operator!= (
        const const_iterator& _another) const
{
    return ! operator==(_another);
}

template <typename T, typename Allocator>
typename list<T, Allocator>::const_iterator::const_reference
list<T, Allocator>::const_iterator::operator*() const
{
    check_dereferencable();
    return reinterpret_cast<const typename list::list_node*>(node)->value;
}

template <typename T, typename Allocator>
typename list<T, Allocator>::const_iterator&
list<T, Allocator>::const_iterator::operator++()
{
    check_valid();
    node = node->next;
    return *this;
}

template <typename T, typename Allocator>
typename list<T, Allocator>::const_iterator&
list<T, Allocator>::const_iterator::operator--()
{
    check_valid();
    node = node->prev;
    return *this;
}

template <typename T, typename Allocator>
typename list<T, Allocator>::const_iterator
list<T, Allocator>::const_iterator::operator++(int)
{
    check_valid();
    const_iterator ret(*this);
    node = node->next;
    return ret;
}

template <typename T, typename Allocator>
typename list<T, Allocator>::const_iterator
list<T, Allocator>::const_iterator::operator--(int)
{
    check_initialized();
    check_valid();
    const_iterator ret(*this);
    node = node->prev;
    return ret;
}

template <typename T, typename Allocator>
list<T, Allocator>::const_iterator::const_iterator(
        const list *_get_from,
        const typename list::list_node_base *_node) :
    get_from(_get_from),
    node(_node),
    validating_ptr(_get_from->validating_ptr)
{
}

template <typename T, typename Allocator>
void
list<T, Allocator>::const_iterator::check_initialized() const noexcept
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
list<T, Allocator>::const_iterator::check_valid() const noexcept
{
    check_initialized();
    if (*(validating_ptr.get()) == false)
    {
        stl_panic(C8_DYN__ITER__DELETED_CONTAINER);
    }

    if (get_from->search_node(node));
    else
    {
        stl_panic(C8_DYN__ITER__OLD_ITERATOR);
    }
}

template <typename T, typename Allocator>
void
list<T, Allocator>::const_iterator::check_dereferencable() const noexcept
{
    check_valid();
    if (node == get_from->head)
    {
        stl_panic(C8_DYN__ITER__ITERATOR_OVERFLOW);
    }
}

template <typename T, typename Allocator>
void
swap(list<T, Allocator>& _a, list<T, Allocator>& _b)
{
    _a.swap(_b);
}

} // namespace saber

#endif // LIST_HPP
