#ifndef UTILITY_HPP
#define UTILITY_HPP

#include "type_traits.hpp"
#include <utility>

namespace saber
{

using std::swap;
using std::pair;

template <typename T>
constexpr T&&
forward(typename remove_reference<T>::type& _t) noexcept
{
    return static_cast<T&&>(_t);
}

template <typename T>
constexpr T&&
forward(typename remove_reference<T>::type&& _t) noexcept
{
    return static_cast<T&&>(_t);
}

template <typename T>
constexpr typename remove_reference<T>::type&&
move(T&& _t) noexcept
{
    return static_cast<typename remove_reference<T>::type&&>(_t);
}

} // namespace saber

#endif // UTILITY_HPP
