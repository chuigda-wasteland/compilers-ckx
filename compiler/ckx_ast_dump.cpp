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
{
    _writer.write_whitespace(_level*2);
    switch (opercode)
    {
    case ckx_op::op_unary_negative:
        _writer.write(reinterpret_cast<const qchar*>("Negative")); break;
    case ckx_op::op_unary_positive:
        _writer.write(reinterpret_cast<const qchar*>("Positive")); break;
    case ckx_op::op_addr:
        _writer.write(reinterpret_cast<const qchar*>("AddressOf")); break;
    case ckx_op::op_deref:
        _writer.write(reinterpret_cast<const qchar*>("Dereference")); break;
    case ckx_op::op_bit_not:
        _writer.write(reinterpret_cast<const qchar*>("BitwiseRevert")); break;
    case ckx_op::op_logic_not:
        _writer.write(reinterpret_cast<const qchar*>("LogicalNot")); break;
    case ckx_op::op_inc:
        _writer.write(reinterpret_cast<const qchar*>("Increment")); break;
    case ckx_op::op_dec:
        _writer.write(reinterpret_cast<const qchar*>("Decrement")); break;
    default:
        assert(false); // What the fuck!
    }
    _writer.write(reinterpret_cast<const qchar*>(" Towards\n"));
    operand->ast_dump(_writer, _level+1);
}


void ckx_ast_subscript_expr::ast_dump(ckx_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*2);
    _writer.write(reinterpret_cast<const qchar*>("Array Subscript Operator\n"));
    base->ast_dump(_writer, _level+1);
    subscript->ast_dump(_writer, _level+1);
}


void ckx_ast_invoke_expr::ast_dump(ckx_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*2);
    _writer.write(reinterpret_cast<const qchar*>("Function Invokation\n"));
    _writer.write_whitespace((_level+1)*2);
    _writer.write(reinterpret_cast<const qchar*>("Invoking\n"));
    this->invokable->ast_dump(_writer, _level+2);
    _writer.write_whitespace((_level+1)*2);
    _writer.write(reinterpret_cast<const qchar*>("With arguments\n"));
    for (auto& arg : args)
        arg->ast_dump(_writer, _level+2);
}


void ckx_ast_extract_expr::ast_dump(ckx_file_writer &_writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_enumerator_expr::ast_dump(ckx_file_writer &_writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_cond_expr::ast_dump(ckx_file_writer& _writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_id_expr::ast_dump(ckx_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*2);
    _writer.write(reinterpret_cast<const qchar*>("Identifier \""));
    _writer.write(name);
    _writer.write(reinterpret_cast<const qchar*>("\"\n"));
}


void ckx_ast_cast_expr::ast_dump(ckx_file_writer &_writer, quint16 _level)
{ Q_UNUSED(_writer); Q_UNUSED(_level); }


void ckx_ast_sizeof_expr::ast_dump(ckx_file_writer &_writer, quint16 _level)
{
    _writer.write_whitespace(_level*2);
    _writer.write(reinterpret_cast<const qchar*>("Sizeof [["));
    _writer.write(type->to_string());
    _writer.write(reinterpret_cast<const qchar*>("]]\n"));
}


void ckx_ast_vi_literal_expr::ast_dump(ckx_file_writer &_writer, quint16 _level)
{
    _writer.write_whitespace(_level*2);
    _writer.write(reinterpret_cast<const qchar*>("Integral literal "));
    _writer.write(val);
    _writer.write(reinterpret_cast<const qchar*>("\n"));
}


void ckx_ast_vr_literal_expr::ast_dump(ckx_file_writer &_writer, quint16 _level)
{
    _writer.write_whitespace(_level*2);
    _writer.write(reinterpret_cast<const qchar*>("Real literal "));
    _writer.write(val);
    _writer.write(reinterpret_cast<const qchar*>("\n"));
}


} // namespace ckx
