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
#include "string_pool.hpp"

namespace ckx
{

open_class ckx_token
{
    enum class type : qchar
    {
        tk_assign,             // =

        tk_add,                // +
        tk_sub,                // -
        tk_mul,                // *
        tk_div,                // /
        tk_mod,                // %

        tk_add_assign,         // +=
        tk_sub_assign,         // -=
        tk_mul_assign,         // *=
        tk_div_assign,         // /=
        tk_mod_assign,         // %=

        tk_bit_and,            // &
        tk_bit_or,             // |
        tk_bit_not,            // ~
        tk_bit_xor,            // ^

        tk_logic_and,          // &&
        tk_logic_or,           // ||
        tk_logic_not,          // !
        tk_logic_xor,          // ^^

        tk_lt,                 // <
        tk_eq,                 // ==
        tk_gt,                 // >
        tk_leq,                // <=
        tk_geq,                // >=
        tk_neq,                // !=

        tk_colon,              // :
        tk_scope,              // ::
        tk_semicolon,          // ;
        tk_comma,              // ,
        tk_dot,                // .

        tk_lbrace,             // {
        tk_rbrace,             // }
        tk_lbracket,           // [
        tk_rbracket,           // ]
        tk_lparth,             // (
        tk_rparth,             // )


        tk_vi8,               // vi8
        tk_vi16,              // vi16
        tk_vi32,              // vi32
        tk_vi64,              // vi64

        tk_vu8,              // vu8
        tk_vu16,             // vu16
        tk_vu32,             // vu32
        tk_vu64,             // vu64

        tk_vch,               // vch

        tk_vr32,             // vr32
        tk_vr64,             // vr64

        tk_void,               // void

        tk_function,           // function

        tk_struct,             // struct
        tk_variant,            // variant
        tk_enum,               // enum

        tk_const,              // const

        tk_if,                 // if
        tk_else,               // else
        tk_for,                // for
        tk_while,              // while
        tk_do,                 // do
        tk_switch,             // switch
        tk_case,               // case
        tk_return,             // return
        tk_break,              // break
        tk_continue,           // continue

        tk_operator,           // operator

        tk_id        ,         // identifier

        tk_vi_literal,         // int literal
        // reserved
        tk_vu_literal,         // unsigned literal
        tk_vr_literal,         // real literal
        tk_vchar_literal,      // char literal
        // reserved
        tk_string_literal,     // string literal

        tk_static_cast,        // static_cast
        tk_reinterpret_cast,   // reinterpret_cast
        tk_const_cast,         // const_cast
        tk_ckx_cast,           // ckx_cast

        tk_ckx,                // ckx
        tk_eoi                 // EOI
    };

    ckx_token(const qcoord& _pos, type _operator);
    ckx_token(const qcoord& _pos, qint64 _int_literal);
    ckx_token(const qcoord& _pos, quint64 _unsigned_literal);
    ckx_token(const qcoord& _pos, qreal _real_literal);
    ckx_token(const qcoord& _pos, qchar _char_literal);
    ckx_token(const qcoord& _pos, saber_string&& _id);

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
    } v;

    qcoord position;
    saber_string_view str;

private:
    // This constructor reserved for internal use.
    // All other public constructors invokes this constructor
    explicit ckx_token(const qcoord& _coord);
};

struct ckx_token_type_hash
{
    size_t operator() (ckx_token::type _type) const
    {
        return std::hash<qchar>()(static_cast<qchar>(_type));
    }
};

}

#endif // CKX_TOKEN_HPP
