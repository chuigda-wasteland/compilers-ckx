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

constexpr quint16 indent_size = 4;

void ckx_ast_translation_unit::ast_dump(we::we_file_writer& _writer,
                                        quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write("Translation-unit\n");
    for (auto &stmt : stmts)
        stmt->ast_dump(_writer, _level+1);
    _writer.write_whitespace(_level*indent_size);
}


void ckx_ast_compound_stmt::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write("Compound-statement\n");
    for (auto &stmt : stmts)
        stmt->ast_dump(_writer, _level+1);
    _writer.write("\n");
}


void ckx_ast_if_stmt::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write("If-statement\n");
    _writer.write_whitespace((_level+1)*indent_size);
    _writer.write("Condition\n");
    condition->ast_dump(_writer, _level+2);
    _writer.write_whitespace((_level+1)*indent_size);
    _writer.write("Then\n");
    then_clause->ast_dump(_writer, _level+2);
    if (else_clause != nullptr)
    {
        _writer.write_whitespace((_level+1)*indent_size);
        _writer.write("Else\n");
        else_clause->ast_dump(_writer, _level+2);
    }
}


void ckx_ast_while_stmt::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write("While-statement\n");
    _writer.write_whitespace((_level+1)*indent_size);
    _writer.write("Condition\n");
    condition->ast_dump(_writer, _level+2);
    _writer.write_whitespace((_level+1)*indent_size);
    _writer.write("Loop\n");
    clause->ast_dump(_writer, _level+2);
}


void ckx_ast_do_while_stmt::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write("Do-while-statement\n");
    _writer.write_whitespace((_level+1)*indent_size);
    _writer.write("Condition\n");
    condition->ast_dump(_writer, _level+2);
    _writer.write_whitespace((_level+1)*indent_size);
    _writer.write("Loop\n");
    clause->ast_dump(_writer, _level+2);
}


void ckx_ast_for_stmt::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write("For-statement\n");
    if (init != nullptr)
    {
        _writer.write_whitespace((_level+1)*indent_size);
        _writer.write("Initialization\n");
        init->ast_dump(_writer, _level+2);
    }
    if (condition != nullptr)
    {
        _writer.write_whitespace((_level+1)*indent_size);
        _writer.write("Condition\n");
        condition->ast_dump(_writer, _level+2);
    }
    if (incr != nullptr)
    {
        _writer.write_whitespace((_level+1)*indent_size);
        _writer.write("Increment\n");
        incr->ast_dump(_writer, _level+2);
    }
    _writer.write_whitespace((_level+1)*indent_size);
    _writer.write("Loop\n");
    clause->ast_dump(_writer, _level+2);
}


void ckx_ast_break_stmt::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write("Break-statement\n");
}


void ckx_ast_continue_stmt::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write("Continue-statement\n");
}


void ckx_ast_return_stmt::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write("Return-statement\n");
    if (return_expr != nullptr)
    {
        _writer.write_whitespace((_level+1)*indent_size);
        _writer.write("Returns \n");
        return_expr->ast_dump(_writer, _level+2);
    }
}


void ckx_ast_decl_stmt::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write("Declaration\n");
    _writer.write_whitespace((_level+1)*indent_size);
    _writer.write("Type [[");
    _writer.write(type->to_string());
    _writer.write("]]\n");
    for (auto &decl : decls)
    {
        _writer.write_whitespace((_level+2)*indent_size);
        _writer.write("\"");
        _writer.write(decl.name);
        _writer.write("\"");
        if (decl.init)
        {
            _writer.write("Initialized with \n");
            decl.init->ast_dump(_writer, _level+3);
        }
        else
        {
            _writer.write("\n");
        }
    }
}


void ckx_ast_expr_stmt::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write("Expression-statement\n");
    expr->ast_dump(_writer, _level+1);
}


void ckx_ast_func_stmt::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write("Function \"");
    _writer.write(name);
    _writer.write("\"\n");
    _writer.write_whitespace((_level+1)*indent_size);
    _writer.write("Takes\n");
    for (auto &param_decl : param_decls)
    {
        _writer.write_whitespace((_level+2)*indent_size);
        _writer.write("\"");
        _writer.write(param_decl.name);
        _writer.write("\" Of type [[");
        _writer.write(param_decl.type->to_string());
        _writer.write("]] \n");
    }
    _writer.write_whitespace((_level+1)*indent_size);
    _writer.write("Returns [[");
    _writer.write(ret_type->to_string());
    _writer.write("]]\n");
    if (fnbody != nullptr)
    {
        _writer.write_whitespace((_level+1)*indent_size);
        _writer.write("Function body\n");
        fnbody->ast_dump(_writer, _level+2);
    }
}


void ckx_ast_struct_stmt::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write("Struct \"");
    _writer.write(name);
    _writer.write("\" contains\n");
    for (auto &field : fields)
    {
        _writer.write_whitespace((_level+1)*indent_size);
        _writer.write("Field \"");
        _writer.write(field.name);
        _writer.write("\" of type [[");
        _writer.write(field.type->to_string());
        _writer.write("]]\n");
    }
}


void ckx_ast_variant_stmt::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write("Variant \"");
    _writer.write(name);
    _writer.write("\" contains\n");
    for (auto &field : fields)
    {
        _writer.write_whitespace((_level+1)*indent_size);
        _writer.write("Field \"");
        _writer.write(field.name);
        _writer.write("\" of type [[");
        _writer.write(field.type->to_string());
        _writer.write("]]\n");
    }
}


void ckx_ast_alias_stmt::ast_dump(we::we_file_writer &_writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write("Alias \"");
    _writer.write(name);
    _writer.write("\" to [[");
    _writer.write(type->to_string());
    _writer.write("]]\n");
}


void ckx_ast_enum_stmt::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write("Enumeration \"");
    _writer.write(name);
    _writer.write("\" contains\n");
    for (auto &enumerator : enumerators)
    {
        _writer.write_whitespace((_level+1)*indent_size);
        _writer.write("Enumerator \"");
        _writer.write(enumerator.name);
        _writer.write("\" = ");
        _writer.write(enumerator.value);
        _writer.write("\n");
    }
}


void ckx_ast_binary_expr::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("Operator "));
    switch (opercode)
    {
    case ckx_op::op_add:
        _writer.write(reinterpret_cast<const qchar*>("+")); break;
    case ckx_op::op_sub:
        _writer.write(reinterpret_cast<const qchar*>("-")); break;
    case ckx_op::op_mul:
        _writer.write(reinterpret_cast<const qchar*>("*")); break;
    case ckx_op::op_div:
        _writer.write(reinterpret_cast<const qchar*>("/")); break;
    case ckx_op::op_mod:
        _writer.write(reinterpret_cast<const qchar*>("%")); break;
    case ckx_op::op_bit_and:
        _writer.write(reinterpret_cast<const qchar*>("&")); break;
    case ckx_op::op_bit_or:
        _writer.write(reinterpret_cast<const qchar*>("|")); break;
    case ckx_op::op_bit_xor:
        _writer.write(reinterpret_cast<const qchar*>("^")); break;
    case ckx_op::op_logic_and:
        _writer.write(reinterpret_cast<const qchar*>("&&")); break;
    case ckx_op::op_logic_or:
        _writer.write(reinterpret_cast<const qchar*>("||")); break;
    case ckx_op::op_lt:
        _writer.write(reinterpret_cast<const qchar*>("<")); break;
    case ckx_op::op_gt:
        _writer.write(reinterpret_cast<const qchar*>(">")); break;
    case ckx_op::op_eq:
        _writer.write(reinterpret_cast<const qchar*>("==")); break;
    case ckx_op::op_leq:
        _writer.write(reinterpret_cast<const qchar*>("<=")); break;
    case ckx_op::op_geq:
        _writer.write(reinterpret_cast<const qchar*>(">=")); break;
    case ckx_op::op_neq:
        _writer.write(reinterpret_cast<const qchar*>("!=")); break;

    case ckx_op::op_assign:
        _writer.write(reinterpret_cast<const qchar*>("=")); break;
    case ckx_op::op_add_assign:
        _writer.write(reinterpret_cast<const qchar*>("+=")); break;
    case ckx_op::op_sub_assign:
        _writer.write(reinterpret_cast<const qchar*>("-=")); break;
    case ckx_op::op_mul_assign:
        _writer.write(reinterpret_cast<const qchar*>("*=")); break;
    case ckx_op::op_div_assign:
        _writer.write(reinterpret_cast<const qchar*>("/=")); break;
    case ckx_op::op_mod_assign:
        _writer.write(reinterpret_cast<const qchar*>("%=")); break;

    default: assert(false); // What the fuck!
    }
    _writer.write(reinterpret_cast<const qchar*>("\n"));
    loperand->ast_dump(_writer, _level+1);
    roperand->ast_dump(_writer, _level+1);
}


void ckx_ast_unary_expr::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
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


void ckx_ast_subscript_expr::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("Array Subscript Operator\n"));
    base->ast_dump(_writer, _level+1);
    subscript->ast_dump(_writer, _level+1);
}


void ckx_ast_invoke_expr::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("Function Invokation\n"));
    _writer.write_whitespace((_level+1)*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("Invoking\n"));
    invokable->ast_dump(_writer, _level+2);
    _writer.write_whitespace((_level+1)*indent_size);
    if (args.empty())
    {
        _writer.write(reinterpret_cast<const qchar*>("With no arguments\n"));
    }
    else
    {
        _writer.write(reinterpret_cast<const qchar*>("With arguments\n"));
        for (auto& arg : args)
            arg->ast_dump(_writer, _level+2);
    }
}


void ckx_ast_extract_expr::ast_dump(we::we_file_writer &_writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("Extraction\n"));
    extracted->ast_dump(_writer, _level+1);
    _writer.write_whitespace((_level+1)*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("Field \""));
    _writer.write(field_name);
    _writer.write(reinterpret_cast<const qchar*>("\"\n"));
}


void ckx_ast_enumerator_expr::ast_dump(we::we_file_writer &_writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("Enumerator\n"));
    _writer.write_whitespace((_level+1)*indent_size);
    _writer.write(enum_name);
    _writer.write(reinterpret_cast<const qchar*>(" :: "));
    _writer.write(enumerator_name);
    _writer.write(reinterpret_cast<const qchar*>("\n"));
}


void ckx_ast_cond_expr::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("Conditional Operator\n"));
    _writer.write_whitespace((_level+1)*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("Condi\n"));
    cond_expr->ast_dump(_writer, _level+2);
    _writer.write_whitespace((_level+1)*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("Then\n"));
    then_expr->ast_dump(_writer, _level+2);
    _writer.write_whitespace((_level+1)*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("Else\n"));
    else_expr->ast_dump(_writer, _level+2);
}


void ckx_ast_id_expr::ast_dump(we::we_file_writer& _writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("Identifier \""));
    _writer.write(name);
    _writer.write(reinterpret_cast<const qchar*>("\"\n"));
}


void ckx_ast_cast_expr::ast_dump(we::we_file_writer &_writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    switch (op)
    {
    case castop::cst_ckx:
        _writer.write(reinterpret_cast<const qchar*>("Ckx Cast\n")); break;
    case castop::cst_const:
        _writer.write(reinterpret_cast<const qchar*>("Const Cast\n")); break;
    case castop::cst_static:
        _writer.write(reinterpret_cast<const qchar*>("Static Cast\n")); break;
    case castop::cst_reinterpret:
        _writer.write(reinterpret_cast<const qchar*>("Reinterpret Cast\n"));
        break;
    }
    _writer.write_whitespace((_level+1)*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("From\n"));
    expr->ast_dump(_writer, _level+2);
    _writer.write_whitespace((_level+1)*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("To\n"));
    _writer.write_whitespace((_level+2)*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("[["));
    _writer.write(this->desired_type->to_string());
    _writer.write(reinterpret_cast<const qchar*>("]]\n"));

}


void ckx_ast_sizeof_expr::ast_dump(we::we_file_writer &_writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("Sizeof [["));
    _writer.write(type->to_string());
    _writer.write(reinterpret_cast<const qchar*>("]]\n"));
}


void ckx_ast_vi_literal_expr::ast_dump(we::we_file_writer &_writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("Integral literal "));
    _writer.write(val);
    _writer.write(reinterpret_cast<const qchar*>("\n"));
}


void ckx_ast_vr_literal_expr::ast_dump(we::we_file_writer &_writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("Real literal "));
    _writer.write(val);
    _writer.write(reinterpret_cast<const qchar*>("\n"));
}


void ckx_ast_array_expr::ast_dump(we::we_file_writer &_writer, quint16 _level)
{
    _writer.write_whitespace(_level*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("Array literal \n"));
    if (size != -1)
    {
        _writer.write_whitespace((_level+1)*indent_size);
        _writer.write(reinterpret_cast<const qchar*>("Of size "));
        _writer.write(static_cast<qint64>(size));
        _writer.write(reinterpret_cast<const qchar*>("\n"));
    }

    if (start != nullptr)
    {
        _writer.write_whitespace((_level+1)*indent_size);
        _writer.write(reinterpret_cast<const qchar*>("Starts at\n"));
        start->ast_dump(_writer, _level+2);
    }

    if (finish != nullptr)
    {
        _writer.write_whitespace((_level+1)*indent_size);
        _writer.write(reinterpret_cast<const qchar*>("Finishes at\n"));
        finish->ast_dump(_writer, _level+2);
    }

    _writer.write_whitespace((_level+1)*indent_size);
    _writer.write(reinterpret_cast<const qchar*>("Initialized with\n"));
    if (!init_list.empty())
    {
        for (auto &item : init_list) item->ast_dump(_writer, _level+2);
    }
    else
    {
        _writer.write_whitespace((_level+2)*indent_size);
        _writer.write(reinterpret_cast<const qchar*>("Nothing\n"));
    }
}

} // namespace ckx
