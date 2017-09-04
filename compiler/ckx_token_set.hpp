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

#ifndef CKX_TOKEN_SET_HPP
#define CKX_TOKEN_SET_HPP

#include "ckx_token.hpp"
#include "unordered_set.hpp"

namespace ckx
{

namespace detail
{

using ckx_token_set = saber::unordered_set<ckx_token::type,
                                           ckx_token_type_hash>;
/**
    @brief The ckx_token_sets class
    static helper class that allows parser to obtain pre-built token_sets
 */
class ckx_token_sets
{
public:
    /// @todo more sets may be added here
    /// after we get a complete error recover strategy

    static const ckx_token_set& first_stmt_set();
    static const ckx_token_set& first_global_set();
    static const ckx_token_set& close_scope_set();
    static const ckx_token_set& close_declarator_set();
    static const ckx_token_set& close_struct_decl_set();
};

} // namespace detail

} // namespace ckx

#endif // CKX_TOKEN_SET_HPP
