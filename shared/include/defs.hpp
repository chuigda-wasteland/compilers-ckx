/**
    The Opensource Compiler CKX -- for my honey ChenKX
    Copyright (C) 2017  CousinZe

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/.
  */

#ifndef DEFS_HPP
#define DEFS_HPP

#include <cstdint>
#include <cstddef>

#include <utility>
#include "string.hpp"

using qint64 = std::int64_t;
using qint32 = std::int32_t;
using qint16 = std::int16_t;
using qint8  = std::int8_t;

using quint64 = std::uint64_t;
using quint32 = std::uint32_t;
using quint16 = std::uint16_t;
using quint8  = std::uint8_t;

using qptrdiff = std::ptrdiff_t;

using qchar = signed char;

using qsizet = size_t;

using qreal = double;

using qcoord = std::pair<qsizet, qsizet>;

template <typename T1, typename T2>
using qpair = std::pair<T1, T2>;

using std::make_pair;

template <typename T1, typename T2, typename T3>
using qtuple = std::tuple<T1, T2, T3>;

template <typename ArrayType>
constexpr qsizet countof(const ArrayType& _array)
{
    return sizeof(_array) / sizeof(_array[0]);
}

inline saber::string to_string_helper(const qcoord& _coord)
{
    return saber::to_string_helper(quint64(_coord.first))
           + saber::to_string_helper(quint64(_coord.second));
}

#define interface class
#define pod_struct struct
#define open_class struct
#define variant union

#define implements : public
#define make_use_of : public

#define Q_ON_HOLD(ANYTHING)

#define COMMENT(COMMENTS)

#endif // DEFS_HPP
