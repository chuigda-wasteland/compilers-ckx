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

static void print_space_indent(FILE* _fp, qint8 _count)
{
    for (qint8 i = 0; i < _count; i++)
        std::fputc('\x20', _fp);
}

void ckx_ast_translation_unit::ast_dump(FILE* _fp, qint8 _level)
{
    print_space_indent(_fp, _level);
    std::fprintf(_fp, "translation-unit\n");
    for (ckx_ast_stmt *stmt : stmts)
        stmt->ast_dump(_fp, _level+1);
}

void ckx_ast_compound_stmt::ast_dump(FILE *_fp, qint8 _level)
{
    print_space_indent(_fp, _level);
    std::fprintf(_fp, "compound-statement\n");
    for (ckx_ast_stmt *stmt : stmts)
        stmt->ast_dump(_fp, _level+1);
}

void ckx_ast_if_stmt::ast_dump(FILE *_fp, qint8 _level)
{
    print_space_indent(_fp, _level);
    std::fprintf(_fp, "if-statement\n");
    print_space_indent(_fp, _level+1);
    std::fprintf(_fp, "if-condition\n");
    condition->ast_dump(_fp, _level+2);
    print_space_indent(_fp, _level+1);
    std::fprintf(_fp, "then-clause\n");
    then_clause->ast_dump(_fp, _level+2);
    print_space_indent(_fp, _level+1);
    std::fprintf(_fp, "else-clause\n");
    else_clause->ast_dump(_fp, _level+2);
}

void ckx_ast_while_stmt::ast_dump(FILE *_fp, qint8 _level)
{
    print_space_indent(_fp, _level);
    std::fprintf(_fp, "while-statement\n");
    print_space_indent(_fp, _level+1);
    std::fprintf(_fp, "while-condition\n");
    condition->ast_dump(_fp, _level+2);
    print_space_indent(_fp, _level+1);
    std::fprintf(_fp, "clause\n");
    clause->ast_dump(_fp, _level+2);
}

void ckx_ast_do_while_stmt::ast_dump(FILE *_fp, qint8 _level)
{
    print_space_indent(_fp, _level);
    std::fprintf(_fp, "do-while-statement\n");
    print_space_indent(_fp, _level+1);
    std::fprintf(_fp, "do-while-condition\n");
    condition->ast_dump(_fp, _level+2);
    print_space_indent(_fp, _level+1);
    std::fprintf(_fp, "clause\n");
    clause->ast_dump(_fp, _level+2);
}

void ckx_ast_break_stmt::ast_dump(FILE *_fp, qint8 _level)
{
    print_space_indent(_fp, _level);
    std::fprintf(_fp, "break\n");
}

void ckx_ast_continue_stmt::ast_dump(FILE *_fp, qint8 _level)
{
    print_space_indent(_fp, _level);
    std::fprintf(_fp, "continue\n");
}

void ckx_ast_return_stmt::ast_dump(FILE *_fp, qint8 _level)
{
    print_space_indent(_fp, _level);
    std::fprintf(_fp, "return\n");
    return_expr->ast_dump(_fp, _level);
}

void ckx_ast_decl_stmt::ast_dump(FILE *_fp, qint8 _level)
{
    print_space_indent(_fp, _level);
    std::fprintf(_fp, "declarations\n");
    for (ckx_ast_init_decl *decl : decls)
        decl->ast_dump(_fp, _level+1);
}

void ckx_ast_expr_stmt::ast_dump(FILE *_fp, qint8 _level)
{
    /// @todo refill after implementing expr system
    Q_UNUSED(_fp);
    Q_UNUSED(_level);
}

void ckx_ast_func_stmt::ast_dump(FILE *_fp, qint8 _level)
{
    print_space_indent(_fp, _level);
    std::fprintf(_fp, "function\n");
    fnbody->ast_dump(_fp, _level+1);
}

void ckx_ast_init_decl::ast_dump(FILE *_fp, qint8 _level)
{
    /// @todo perhaps we need to make entries store names.
    /// and make a set of functions for converting types to strings.
    Q_UNUSED(_fp);
    Q_UNUSED(_level);
}

void ckx_ast_param_decl::ast_dump(FILE *_fp, qint8 _level)
{
    /// @todo the same as init_decl
    Q_UNUSED(_fp);
    Q_UNUSED(_level);
}

void ckx_ast_struct_stmt::ast_dump(FILE *_fp, qint8 _level)
{
    Q_UNUSED(_fp);
    Q_UNUSED(_level);
}

void ckx_ast_variant_stmt::ast_dump(FILE *_fp, qint8 _level)
{
    Q_UNUSED(_fp);
    Q_UNUSED(_level);
}

void ckx_ast_enum_stmt::ast_dump(FILE *_fp, qint8 _level)
{
    Q_UNUSED(_fp);
    Q_UNUSED(_level);
}

} // namespace ckx
