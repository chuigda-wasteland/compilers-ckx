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
    ckx_token(_pos)
{
    this->token_type = _operator;
}

ckx_token::ckx_token(const qcoord &_pos, qint64 _int_literal) :
    ckx_token(_pos)
{
    this->token_type = type::tk_vi_literal;
    this->v.i64 = _int_literal;
}

ckx_token::ckx_token(const qcoord &_pos, quint64 _unsigned_literal) :
    ckx_token(_pos)
{
    this->token_type = type::tk_vu_literal;
    this->v.u64 = _unsigned_literal;
}

ckx_token::ckx_token(const qcoord &_pos, qreal _real_literal) :
    ckx_token(_pos)
{
    this->token_type = type::tk_vr_literal;
    this->v.r = _real_literal;
}

ckx_token::ckx_token(const qcoord &_pos, qchar _char_literal) :
    ckx_token(_pos)
{
    this->token_type = type::tk_vchar_literal;
    this->v.ch = _char_literal;
}

ckx_token::ckx_token(const qcoord &_pos, saber_string_view _id) :
    ckx_token(_pos)
{
    token_type = type::tk_id;
    str = _id;
}

ckx_token::~ckx_token()
{
}

ckx_token::ckx_token(const qcoord &_coord) :
    position(_coord),
    str(saber_string_pool::create_view(""))
{}


saber_string to_string_helper(saber_ptr<ckx_token> _token)
{
    switch (_token->token_type)
    {
#   define GG2LEX(X, Y) \
    case ckx_token::type::tk_##Y: return X;
#   define GGLEX(X, Y) \
    case ckx_token::type::tk_##Y: return X;
#   include "gg.h"
#   undef GG2LEX
#   undef GGLEX
    case ckx_token::type::tk_vi_literal:
        return saber::to_string_helper(_token->v.i64);
    case ckx_token::type::tk_vr_literal:
        return saber::to_string_helper(_token->v.r);
    case ckx_token::type::tk_vu_literal:
        return saber::to_string_helper(_token->v.u64);
    case ckx_token::type::tk_vchar_literal:
        return saber::to_string_helper(char(_token->v.ch));
    case ckx_token::type::tk_string_literal:
    case ckx_token::type::tk_id:
        return saber::to_string_helper(_token->str);
    case ckx_token::type::tk_eoi:
        return "EOI";
    }
    assert(false);
}

} // namespace ckx;
