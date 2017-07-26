#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

// This part is even rawly copied from EA-STL since I don't know much about
// functional library. However, this may be changed soon.

#include "safe_stl_general.hpp"
#include "utility.hpp"

namespace saber
{

struct allocator_arg_t {};
constexpr allocator_arg_t allocator_arg = allocator_arg_t();

template <typename Argument, typename Result>
struct unary_function
{
    using argument_type = Argument;
    using result_type = Result;
};

template <typename Argument1, typename Argument2, typename Result>
struct binary_function
{
    using first_argument_type = Argument1;
    using second_argument_type = Argument2;
    using result_type = Result;
};

template <typename T = void>
struct less : public binary_function<T, T, bool>
{
    constexpr bool operator() (const T& _lhs, const T& _rhs) const
        { return _lhs < _rhs; }
};

template<>
struct less<void>
{
    template <typename A, typename B>
    constexpr auto operator() (A&& _lhs, B&& _rhs) const
        -> decltype(saber::forward<A>(_lhs) < saber::forward<B>(_rhs))
    {
        return saber::forward<A>(_lhs) < saber::forward<B>(_rhs);
    }
};


// just for fun.
template <typename T = void, typename LessPredicator = void>
struct c8_equivalent
{
    constexpr bool operator() (const T& _a, const T& _b,
                               const LessPredicator& _less_pred) const
    { return (!_less_pred(_a, _b)) && (!_less_pred(_b, _a)); }
};

template <>
struct c8_equivalent<void, void>
{
    template <typename A, typename B, typename LessPredicator>
    constexpr auto operator() (A&& _a, B&& _b,
                               const LessPredicator& _less_pred) const
        ->
        decltype( (! _less_pred(saber::forward<A>(_a), saber::forward<B>(_b)) ) &&
                  (! _less_pred(saber::forward<B>(_b), saber::forward<A>(_a)) ) )
    { return (!_less_pred(saber::forward<A>(_a), saber::forward<B>(_b)))
          && (!_less_pred(saber::forward<B>(_b), saber::forward<A>(_a))); }
};

} // namespace saber

#endif // FUNCTIONAL_HPP
