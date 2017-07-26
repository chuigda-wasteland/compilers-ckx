#ifdef _MSC_VER
#pragma once
#endif

#ifndef EXCALIBUR_HPP
#define EXCALIBUR_HPP

#include <iostream>
#include "safe_stl_general.hpp"
#include "utility.hpp"

namespace saber
{

template <typename T>
void
excalibur(const T* _t)
{
    for (size_t i = 0; i < sizeof(T); ++i)
    {
        if (i % 8 == 0)
        {
            std::putchar('\n');
        }

        std::printf("%02x ", (unsigned)
                            (*reinterpret_cast<unsigned char*>
                            (const_cast<T*>(_t + i))));
    }
    std::putchar('\n');
}

template <typename InputIterator>
void
avalon(InputIterator _begin, InputIterator _end)
{
    for (;_begin != _end; ++_begin)
    {
        std::cout << *_begin << ' ';
    }
    std::cout << std::endl;
}

template <typename T>
void
lawaxis(T&& _t1, T&& _t2)
{
    if (saber::forward<T>(_t1) != std::forward<T>(_t2))
    {
        stl_panic("LawAxis error!");
    }
}

template <typename InputIterator1, typename InputIterator2>
void
lawaxis(InputIterator1 _first, InputIterator1 _last,
        InputIterator2 _d_first, InputIterator2 _d_last)
{
    while (_first != _last && _d_first != _d_last)
    {
        if (*_first != *_d_first)
        {
            stl_panic("LawAxis error!");
        }
        ++_first; ++_d_first;
    }
}

} // namespace saber

#endif // EXCALIBUR_HPP
