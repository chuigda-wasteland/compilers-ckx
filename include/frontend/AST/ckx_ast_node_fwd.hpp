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

#ifndef CKX_AST_NODE_FWD_HPP
#define CKX_AST_NODE_FWD_HPP

namespace ckx
{

class ckx_ast_node;
class ckx_ast_translation_unit;
class ckx_ast_stmt;

    class ckx_ast_compound_stmt;
    class ckx_ast_if_stmt;
    class ckx_ast_while_stmt;
    class ckx_ast_for_stmt;
    class ckx_ast_do_while_stmt;
    class ckx_ast_break_stmt;
    class ckx_ast_continue_stmt;
    class ckx_ast_return_stmt;

    class ckx_ast_record_stmt;
    class ckx_ast_enum_stmt;
    class ckx_ast_alias_stmt;
    class ckx_ast_decl_stmt;
    class ckx_ast_func_stmt;
    class ckx_ast_expr_stmt;

class ckx_ast_init_decl;

class ckx_ast_expr;
    class ckx_ast_binary_expr;
    class ckx_ast_unary_expr;
    class ckx_ast_subscript_expr;
    class ckx_ast_invoke_expr;
    class ckx_ast_extract_expr;
    class ckx_ast_enumerator_expr;
    class ckx_ast_cond_expr;
    class ckx_ast_id_expr;
    class ckx_ast_cast_expr;
    class ckx_ast_sizeof_expr;
    class ckx_ast_vi_literal_expr;
    class ckx_ast_vr_literal_expr;
    class ckx_ast_nullptr_expr;
    class ckx_ast_bool_literal_expr;
    class ckx_ast_array_expr;

} // namespace ckx

#endif // CKX_AST_NODE_FWD_HPP
