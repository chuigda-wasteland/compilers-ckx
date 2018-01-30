#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

namespace saber
{

template <typename T>
struct remove_reference { using type = T; };

template <typename T>
struct remove_reference<T&> { using type = T; };

template <typename T>
struct remove_reference<T&&> { using type = T; };

} // namespace saber

#endif // TYPE_TRAITS_HPP
