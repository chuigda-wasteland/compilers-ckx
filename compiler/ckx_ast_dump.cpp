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

#include <cstdio>
#include "ckx_ast_node.hpp"

namespace ckx
{

static void print_space_indent(ckx_file_writer& _writer, quint16 _count)
{ Q_UNUSED(_writer); Q_UNUSED(_count); }


void ckx_ast_translation_unit::ast_dump(ckx_file_writer& _writer,
                                        quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_compound_stmt::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_if_stmt::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_while_stmt::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_do_while_stmt::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_for_stmt::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_break_stmt::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_continue_stmt::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_return_stmt::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_decl_stmt::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_expr_stmt::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_func_stmt::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_init_decl::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_struct_stmt::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_variant_stmt::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_enum_stmt::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_binary_expr::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_unary_expr::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_subscript_expr::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_invoke_expr::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_cond_expr::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_id_expr::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_cast_expr::ast_dump(ckx_file_writer &_writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_sizeof_expr::ast_dump(ckx_file_writer &_writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }

void ckx_ast_vi_literal_expr::ast_dump(ckx_file_writer &_writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }

void ckx_ast_vr_literal_expr::ast_dump(ckx_file_writer &_writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }

} // namespace ckx
