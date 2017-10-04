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
    str(saber_string_pool::get().create_view(""))
{}

} // namespace ckx;
