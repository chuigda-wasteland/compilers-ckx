#include "string_pool.hpp"
#include "assert.h"
#include "utility.hpp"
#include "memory.hpp"

namespace saber
{

template <typename StringType>
string_view::string_view(pool_type::set_type::const_iterator _iter) :
    iter(_iter)
{}

template <typename StringType>
string_view::~string_view()
{
    iter->second--;
    if (!iter->second) pool_type::get().string_set.erase(iter);
}

template <typename StringType>
const StringType&
string_view<StringType>::get() const noexcept
{
    return iter->first;
}

template <typename StringType>
bool
string_view<StringType>::operator== (const string_view& _another) const noexcept
{
    return iter == _another.iter;
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
string_pool<StringType>::create_view(StringType &&_string)
{
    auto it = string_set.find(_string);
    if ( it == string_set.end() )
        it = string_set.insert(std::make_pair(saber::move(_string), 1));
    return string_view<StringType>(it);
}

template<> class string_view<string>;
template<> class string_view<wstring>;
template<> class string_view<u16string>;
template<> class string_view<u32string>;

} // namespace saber
