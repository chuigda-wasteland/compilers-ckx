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

#include "ckx_token_set.hpp"

namespace ckx
{

namespace detail
{

const ckx_token_set&
ckx_token_sets::first_stmt_set()
{
    static ckx_token_set ret =
    {
        /// @brief elements may appear at the commence of a declaration
        ckx_token::type::token_identifier,
        ckx_token::type::token_int8,
        ckx_token::type::token_int16,
        ckx_token::type::token_int32,
        ckx_token::type::token_int64,
        ckx_token::type::token_uint8,
        ckx_token::type::token_uint16,
        ckx_token::type::token_uint32,
        ckx_token::type::token_uint64,
        ckx_token::type::token_char,
        ckx_token::type::token_real32,
        ckx_token::type::token_real64,

        /// @brief elements may appear at the commence of an expression
        ckx_token::type::token_add,
        ckx_token::type::token_sub,
        ckx_token::type::token_mul,
        ckx_token::type::token_bit_and,
        ckx_token::type::token_const_cast,
        ckx_token::type::token_reinterpret_cast,
        ckx_token::type::token_static_cast,
        ckx_token::type::token_ckx_cast,

        /// @brief control-statements
        ckx_token::type::token_if,
        ckx_token::type::token_while,
        ckx_token::type::token_do,
        ckx_token::type::token_for,
        ckx_token::type::token_break,
        ckx_token::type::token_continue,
        ckx_token::type::token_return,

        /// @brief may enter new scope
        ckx_token::type::token_lbrace
    };
    return ret;
}

const ckx_token_set&
ckx_token_sets::first_global_set()
{
    static ckx_token_set ret =
    {
        /// @brief import/export
        /// @todo remember to add the two keywords
        /// after adding import/export to ckx.

        /// @brief elements may appear at the commence of a declaration
        /// identifiers may be typenames.
        ckx_token::type::token_identifier,
        ckx_token::type::token_int8,
        ckx_token::type::token_int16,
        ckx_token::type::token_int32,
        ckx_token::type::token_int64,
        ckx_token::type::token_uint8,
        ckx_token::type::token_uint16,
        ckx_token::type::token_uint32,
        ckx_token::type::token_uint64,
        ckx_token::type::token_char,
        ckx_token::type::token_real32,
        ckx_token::type::token_real64,

        /// @brief function
        ckx_token::type::token_function,

        /// @brief new-type statements
        ckx_token::type::token_struct,
        ckx_token::type::token_variant,
        ckx_token::type::token_enum
    };
    return ret;
}

const ckx_token_set&
ckx_token_sets::close_scope_set()
{
    static ckx_token_set ret = { ckx_token::type::token_rbrace };
    return ret;
}

const ckx_token_set&
ckx_token_sets::close_declarator_set()
{
    static ckx_token_set ret =
    {
        /// @brief usual termination
        ckx_token::type::token_comma,
        ckx_token::type::token_semicolon,
        ckx_token::type::token_identifier,

        /// @brief any element may appear at next line
        /// if the current statement is not closed

        /// @brief elements may appear at the commence of a declaration
        ckx_token::type::token_identifier,
        ckx_token::type::token_int8,
        ckx_token::type::token_int16,
        ckx_token::type::token_int32,
        ckx_token::type::token_int64,
        ckx_token::type::token_uint8,
        ckx_token::type::token_uint16,
        ckx_token::type::token_uint32,
        ckx_token::type::token_uint64,
        ckx_token::type::token_char,
        ckx_token::type::token_real32,
        ckx_token::type::token_real64,

        /// @brief elements may appear at the commence of an expression
        ckx_token::type::token_add,
        ckx_token::type::token_sub,
        ckx_token::type::token_mul,
        ckx_token::type::token_bit_and,
        ckx_token::type::token_const_cast,
        ckx_token::type::token_reinterpret_cast,
        ckx_token::type::token_static_cast,
        ckx_token::type::token_ckx_cast,

        /// @brief control-statements
        ckx_token::type::token_if,
        ckx_token::type::token_while,
        ckx_token::type::token_do,
        ckx_token::type::token_for,
        ckx_token::type::token_break,
        ckx_token::type::token_continue,
        ckx_token::type::token_return,

        /// @brief may enter new scope
        ckx_token::type::token_lbrace
    };
    return ret;
}

} // namespace detail

} // namespace detail
