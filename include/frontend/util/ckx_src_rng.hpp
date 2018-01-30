#ifndef CKX_SRC_RNG_HPP
#define CKX_SRC_RNG_HPP

#include "we/defs.hpp"

namespace ckx
{

open_class ckx_src_rng
{
    const quint16 begin_line;
    const quint16 begin_col;
    const quint16 end_line;
    const quint16 end_col;

    ckx_src_rng(quint16 _begin_line, quint16 _begin_col,
                quint16 _end_line, quint16 _end_col)
        : begin_line(_begin_line), begin_col(_begin_col),
          end_line(_end_line), end_col(_end_col) {}

    ckx_src_rng(qcoord _begin_coord, qcoord _end_coord)
        : ckx_src_rng(_begin_coord.line, _begin_coord.col,
                      _end_coord.line, _end_coord.col) {}

    static ckx_src_rng concat(ckx_src_rng r1, ckx_src_rng r2)
    {
        C8ASSERT(r2.end_line == r1.end_line ?
                 r2.end_col >= r1.end_col : r2.end_line > r1.end_line);
        return ckx_src_rng(r1.begin_line, r1.begin_col,
                           r2.end_line, r2.end_col);
    }

    static ckx_src_rng empty()
    {
        return ckx_src_rng(-1, -1, -1, -1);
    }
};

} // namespace ckx

#endif // CKX_SRC_RNG_HPP
