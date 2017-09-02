#ifdef _MSC_VER
#pragma once
#endif

#ifndef SABER_TRAITS_HPP
#define SABER_TRAITS_HPP

#include <type_traits>
namespace saber
{
namespace traits
{

using std::true_type;
using std::false_type;

template<typename... Ts> struct make_void { typedef void type;};
template<typename... Ts> using void_t = typename make_void<Ts...>::type;

template <bool b>
struct int2type 
{
    using value_type = false_type;
};

template<>
struct int2type<true>
{
    using value_type = true_type;
};

template <typename T1, typename T2>
struct type_equivalent
{
    using value_type = false_type;
    static constexpr bool value = false;
};

template <typename T>
struct type_equivalent<T, T>
{
    using value_type = true_type;
    static constexpr bool value = true;
};

} // namespace traits
} // namespace saber

#endif // SABER_TRAITS_HPP
