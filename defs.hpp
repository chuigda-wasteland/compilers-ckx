#ifndef DEFS_HPP
#define DEFS_HPP

#include <cstdint>
#include <cstddef>

#include <utility>

namespace ckx
{

using qint64 = std::int64_t;
using qint32 = std::int32_t;
using qint16 = std::int16_t;
using qint8  = std::int8_t;

using quint64 = std::uint64_t;
using quint32 = std::uint32_t;
using quint16 = std::uint16_t;
using quint8  = std::uint8_t;

using qchar = unsigned char;

using qsizet = size_t;
using qptrdiff = std::ptrdiff_t;

using qreal = double;

using qcoord = std::pair<qsizet, qsizet>;

} // namespace ckx

#endif // DEFS_HPP
