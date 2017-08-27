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


#ifndef CKX_TOKEN_HPP
#define CKX_TOKEN_HPP

#include "defs.hpp"
#include "string.hpp"

namespace ckx
{

open_class ckx_token
{
    enum class type : unsigned char
    {
        token_assign,             // =

        token_add,                // +
        token_sub,                // -
        token_mul,                // *
        token_div,                // /
        token_mod,                // %

        token_add_assign,         // +=
        token_sub_assign,         // -=
        token_mul_assign,         // *=
        token_div_assign,         // /=
        token_mod_assign,         // %=

        token_bit_and,            // &
        token_bit_or,             // |
        token_bit_not,            // ~
        token_bit_xor,            // ^

        token_logic_and,          // &&
        token_logic_or,           // ||
        token_logic_not,          // !
        token_logic_xor,          // ^^

        token_lt,                 // <
        token_eq,                 // ==
        token_gt,                 // >
        token_leq,                // <=
        token_geq,                // >=
        token_neq,                // !=

        token_colon,              // :
        token_scope,              // ::
        token_semicolon,          // ;
        token_comma,              // ,
        token_dot,                // .

        token_lbrace,             // {
        token_rbrace,             // }
        token_lbracket,           // [
        token_rbracket,           // ]
        token_lparth,             // (
        token_rparth,             // )


        token_int8,               // vi8
        token_int16,              // vi16
        token_int32,              // vi32
        token_int64,              // vi64

        token_uint8,              // vu8
        token_uint16,             // vu16
        token_uint32,             // vu32
        token_uint64,             // vu64

        token_char,               // vch

        token_real32,             // vr32
        token_real64,             // vr64

        token_void,               // void

        token_function,           // function

        token_struct,             // struct
        token_variant,            // variant
        token_enum,               // enum

        token_const,              // const

        token_if,                 // if
        token_else,               // else
        token_for,                // for
        token_while,              // while
        token_do,                 // do
        token_switch,             // switch
        token_case,               // case
        token_return,             // return
        token_break,              // break
        token_continue,           // continue

        token_operator,           // operator

        token_identifier,         // identifier

        token_vi_literal,         // int literal
        // reserved
        token_vu_literal,         // unsigned literal
        token_vr_literal,         // real literal
        token_vchar_literal,      // char literal
        // reserved
        token_string_literal,     // string literal

        token_static_cast,        // static_cast
        token_reinterpret_cast,   // reinterpret_cast
        token_const_cast,         // const_cast
        token_ckx_cast,           // ckx_cast

        token_ckx,                // ckx
        token_eoi                 // EOI
    };

    ckx_token(const qcoord& _pos, type _operator);
    ckx_token(const qcoord& _pos, qint64 _int_literal);
    ckx_token(const qcoord& _pos, quint64 _unsigned_literal);
    ckx_token(const qcoord& _pos, qreal _real_literal);
    ckx_token(const qcoord& _pos, qchar _char_literal);
    ckx_token(const qcoord& _pos, saber::string&& _id);

    ckx_token() = delete;
    ckx_token(const ckx_token&) = delete;
    ckx_token& operator= (const ckx_token&) = delete;

    ~ckx_token();

    type token_type;

    variant
    {
        qchar ch;
        qint64 i64;
        quint64 u64;
        qreal r;
        saber::string *p_str;
    }
    v;

    qcoord position;

private:
    ckx_token(const qcoord& _coord);
};

}

#endif // CKX_TOKEN_HPP
