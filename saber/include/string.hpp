#ifndef STRING_HPP
#define STRING_HPP

#include <string>
#include <sstream>
#include <memory.hpp>

namespace saber
{
using std::basic_string;
using std::string;

using std::char_traits;

template <typename T>
string to_string_helper(T&& _value)
{
    return saber::forward<T>(_value).to_string();
}

inline string to_string_helper(int64_t _number)
{
    std::stringstream stream;
    stream << _number;
    return stream.str();
}

inline string to_string_helper(uint64_t _number)
{
    std::stringstream stream;
    stream << _number;
    return stream.str();
}

inline string to_string_helper(double _number)
{
    std::stringstream stream;
    stream << _number;
    return stream.str();
}

inline string to_string_helper(char _ch)
{
    string ret; ret.push_back(_ch); return ret;
}

inline string to_string_helper(const char* _str)
{
    return string(_str);
}

inline string to_string_helper(const string& _str)
{
    return string(_str);
}

inline string to_string_helper(string&& _str)
{
    return string(saber::move(_str));
}


inline void paste_impl(const string&)
{
    /// @nattention pass
}

template <typename T, typename... Args>
void paste_impl(string& _str, T&& _value, Args&& ..._args)
{
    _str += to_string_helper(saber::forward<T>(_value));
    paste_impl(_str, saber::forward<Args>(_args)...);
}

template <typename... Args>
string string_paste(Args&& ..._args)
{
    string ret = "";
    paste_impl(ret, saber::forward<Args>(_args)...);
    return ret;
}

}

#endif // STRING_HPP
