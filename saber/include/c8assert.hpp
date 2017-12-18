#ifndef C8ASSERT_HPP
#define C8ASSERT_HPP

#include "safe_stl_general.hpp"

namespace saber
{
#define C8ASSERT(TEST) {if(!(TEST)) stl_panic("Assertion " #TEST " failed.") }
} // namespace saber

#endif // C8ASSERT_HPP
