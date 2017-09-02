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
        ckx_token::type::tk_id,
        ckx_token::type::tk_vi8,
        ckx_token::type::tk_vi16,
        ckx_token::type::tk_vi32,
        ckx_token::type::tk_vi64,
        ckx_token::type::tk_vu8,
        ckx_token::type::tk_vu16,
        ckx_token::type::tk_vu32,
        ckx_token::type::tk_vu64,
        ckx_token::type::tk_vch,
        ckx_token::type::tk_vr32,
        ckx_token::type::tk_vr64,

        /// @brief elements may appear at the commence of an expression
        ckx_token::type::tk_add,
        ckx_token::type::tk_sub,
        ckx_token::type::tk_mul,
        ckx_token::type::tk_bit_and,
        ckx_token::type::tk_const_cast,
        ckx_token::type::tk_reinterpret_cast,
        ckx_token::type::tk_static_cast,
        ckx_token::type::tk_ckx_cast,

        /// @brief control-statements
        ckx_token::type::tk_if,
        ckx_token::type::tk_while,
        ckx_token::type::tk_do,
        ckx_token::type::tk_for,
        ckx_token::type::tk_break,
        ckx_token::type::tk_continue,
        ckx_token::type::tk_return,

        /// @brief may enter new scope
        ckx_token::type::tk_lbrace
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
        ckx_token::type::tk_id,
        ckx_token::type::tk_vi8,
        ckx_token::type::tk_vi16,
        ckx_token::type::tk_vi32,
        ckx_token::type::tk_vi64,
        ckx_token::type::tk_vu8,
        ckx_token::type::tk_vu16,
        ckx_token::type::tk_vu32,
        ckx_token::type::tk_vu64,
        ckx_token::type::tk_vch,
        ckx_token::type::tk_vr32,
        ckx_token::type::tk_vr64,

        /// @brief function
        ckx_token::type::tk_function,

        /// @brief new-type statements
        ckx_token::type::tk_struct,
        ckx_token::type::tk_variant,
        ckx_token::type::tk_enum
    };
    return ret;
}

const ckx_token_set&
ckx_token_sets::close_scope_set()
{
    static ckx_token_set ret = { ckx_token::type::tk_rbrace };
    return ret;
}

const ckx_token_set&
ckx_token_sets::close_declarator_set()
{
    static ckx_token_set ret =
    {
        /// @brief usual termination
        ckx_token::type::tk_comma,
        ckx_token::type::tk_semicolon,
        ckx_token::type::tk_id,

        /// @brief any element may appear at next line
        /// if the current statement is not closed

        /// @brief elements may appear at the commence of a declaration
        ckx_token::type::tk_id,
        ckx_token::type::tk_vi8,
        ckx_token::type::tk_vi16,
        ckx_token::type::tk_vi32,
        ckx_token::type::tk_vi64,
        ckx_token::type::tk_vu8,
        ckx_token::type::tk_vu16,
        ckx_token::type::tk_vu32,
        ckx_token::type::tk_vu64,
        ckx_token::type::tk_vch,
        ckx_token::type::tk_vr32,
        ckx_token::type::tk_vr64,

        /// @brief elements may appear at the commence of an expression
        ckx_token::type::tk_add,
        ckx_token::type::tk_sub,
        ckx_token::type::tk_mul,
        ckx_token::type::tk_bit_and,
        ckx_token::type::tk_const_cast,
        ckx_token::type::tk_reinterpret_cast,
        ckx_token::type::tk_static_cast,
        ckx_token::type::tk_ckx_cast,

        /// @brief control-statements
        ckx_token::type::tk_if,
        ckx_token::type::tk_while,
        ckx_token::type::tk_do,
        ckx_token::type::tk_for,
        ckx_token::type::tk_break,
        ckx_token::type::tk_continue,
        ckx_token::type::tk_return,

        /// @brief may enter new scope
        ckx_token::type::tk_lbrace
    };
    return ret;
}

} // namespace detail

} // namespace detail
