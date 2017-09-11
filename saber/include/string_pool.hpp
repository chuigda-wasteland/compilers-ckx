#ifndef STRING_POOL_HPP
#define STRING_POOL_HPP

#include "string.hpp"
#include "unordered_map.hpp"
#include "memory.hpp"

namespace saber
{

template <typename StringType> class string_pool;



template <typename StringType>
class string_view
{
    friend class string_pool<StringType>;
    using pool_type = string_pool<StringType>;

public:
    string_view(const string_view& _another);
    ~string_view();

    const StringType& get() const noexcept;
    bool operator== (const string_view& _another) const noexcept;
    bool operator!= (const string_view& _another) const noexcept;

private:
    string_view(const StringType* _str = nullptr, size_t* _refcount = nullptr);
    const StringType* str;
    size_t* refcount;
};

template <typename StringType>
class string_pool
{
    friend class string_view<StringType>;
public:
    static string_pool& get() noexcept;
    string_view<StringType> create_view(const StringType& _string);
    string_view<StringType> create_view(StringType&& _string);

private:
    using set_type = unordered_map<StringType, size_t>;

    string_pool() = default;
    set_type string_set;
};



template<typename StringType>
string_view<StringType>::string_view(const StringType *_str,
                                     size_t *_refcount) :
    str(_str),
    refcount(_refcount)
{
}

template <typename StringType>
string_view<StringType>::string_view(const string_view& _another) :
    str(_another.str),
    refcount(_another.refcount)
{
    ++(*refcount);
}

template <typename StringType>
string_view<StringType>::~string_view()
{
    --(*refcount);
    if (*refcount == 0)
        string_pool<StringType>::get().string_set.erase(*str);
}

template <typename StringType>
const StringType&
string_view<StringType>::get() const noexcept
{
    return *str;
}

template <typename StringType>
bool
string_view<StringType>::operator== (const string_view& _another) const noexcept
{
    return string_pool<StringType>::get().string_set.find(*str)
           == string_pool<StringType>::get().string_set.find(*(_another.str));
}

template <typename StringType>
bool
string_view<StringType>::operator!= (const string_view& _another) const noexcept
{
    return ! this->operator==(_another);
}


template <typename StringType>
string_pool<StringType>&
string_pool<StringType>::get() noexcept
{
    static string_pool ret;
    return ret;
}

template <typename StringType>
string_view<StringType>
string_pool<StringType>::create_view(const StringType &_string)
{
    auto it = string_set.find(_string);
    if ( it == string_set.end() )
        it = string_set.insert(std::make_pair(_string, 1)).first;
    return string_view<StringType>(&(it->first), &(it->second));
}

template <typename StringType>
string_view<StringType>
string_pool<StringType>::create_view(StringType &&_string)
{
    auto it = string_set.find(_string);
    if ( it == string_set.end() )
        it = string_set.insert(std::make_pair(saber::move(_string), 1)).first;
    return string_view<StringType>(&(it->first), &(it->second));
}

} // namespace saber

using saber_string = saber::string;
using saber_string_view = saber::string_view<saber::string>;
using saber_string_pool = saber::string_pool<saber::string>;

struct string_view_hash
{
    bool operator() (const saber_string_view& _view)const noexcept
    {
        return std::hash<saber_string>()(_view.get());
    }
};

#endif // STRING_POOL_HPP
