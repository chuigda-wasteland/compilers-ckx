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


#include "ckx_token.hpp"
#include "memory.hpp"
#include "string_pool.hpp"

namespace ckx
{

ckx_token::ckx_token(const qcoord &_pos, type _operator) :
    token_type(_operator),
    v{.i64=0},
    position(_pos)
{}

ckx_token::ckx_token(const qcoord &_pos, qint64 _int_literal) :
    token_type(type::tk_vi_literal),
    v{.i64=_int_literal},
    position(_pos)
{}

ckx_token::ckx_token(const qcoord &_pos, quint64 _unsigned_literal) :
    token_type(type::tk_vr_literal),
    v{.u64=_unsigned_literal},
    position(_pos)
{}

ckx_token::ckx_token(const qcoord &_pos, qreal _real_literal) :
    token_type(type::tk_vr_literal),
    v{.r=_real_literal},
    position(_pos)
{}

ckx_token::ckx_token(const qcoord &_pos, qchar _char_literal) :
    token_type(type::tk_vchar_literal),
    v{.ch=_char_literal},
    position(_pos)
{}

ckx_token::ckx_token(const qcoord &_pos, saber_string_view _id) :
    token_type(type::tk_id),
    v{.i64=0},
    str(_id),
    position(_pos)
{}

ckx_token::~ckx_token()
{}


saber_string to_string_helper(ckx_token _token)
{
    switch (_token.token_type)
    {
#   define GG2LEX(X, Y) \
    case ckx_token::type::tk_##Y: return X;
#   define GGLEX(X, Y) \
    case ckx_token::type::tk_##Y: return X;
#   include "gg.h"
#   undef GG2LEX
#   undef GGLEX
    case ckx_token::type::tk_vi_literal:
        return saber::to_string_helper(_token.v.i64);
    case ckx_token::type::tk_vr_literal:
        return saber::to_string_helper(_token.v.r);
    case ckx_token::type::tk_vu_literal:
        return saber::to_string_helper(_token.v.u64);
    case ckx_token::type::tk_vchar_literal:
        return saber::to_string_helper(char(_token.v.ch));
    case ckx_token::type::tk_string_literal:
    case ckx_token::type::tk_id:
        return saber::to_string_helper(_token.str);
    case ckx_token::type::tk_eoi:
        return "EOI";
    }
    assert(false);
}

} // namespace ckx;
