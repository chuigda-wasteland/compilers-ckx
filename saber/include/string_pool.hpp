#ifndef STRING_POOL_HPP
#define STRING_POOL_HPP

#include "string.hpp"

#include "unordered_map.hpp"

namespace saber
{

template <typename StringType> class string_pool;



template <typename StringType>
class string_view
{
    friend class string_pool<StringType>;
    using pool_type = string_pool<StringType>;
public:
    ~string_view();

    const StringType& get() const noexcept;
    bool operator== (const string_view& _another) const noexcept;
    bool operator!= (const string_view& _another) const noexcept;

private:
    string_view(pool_type::set_type::const_iterator _iter);
    pool_type::set_type::const_iterator iter;
};



template <typename StringType>
class string_pool
{
    friend class string_view<StringType>;
public:
    static string_pool& get() noexcept;
    string_view<StringType> create_view(StringType&& _string);

private:
    using set_type = unordered_map<StringType, size_t>;

    string_pool() = default;
    set_type string_set;
};

extern template<> class string_view<string>;
extern template<> class string_view<wstring>;
extern template<> class string_view<u16string>;
extern template<> class string_view<u32string>;

} // namespace saber

#endif // STRING_POOL_HPP
