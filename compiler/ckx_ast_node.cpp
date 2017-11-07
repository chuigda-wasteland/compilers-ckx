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


#include "ckx_ast_node.hpp"

namespace ckx
{

ckx_ast_node::ckx_ast_node(saber_ptr<ckx_token> _at_token) :
    at_token(_at_token)
{}

saber_ptr<ckx_token> ckx_ast_node::get_at_token()
{ return at_token; }

ckx_ast_translation_unit::ckx_ast_translation_unit(
        saber_ptr<ckx_token> _at_token) :
    ckx_ast_node(_at_token),
    global_table(new ckx_env(nullptr))
{}

ckx_ast_translation_unit::~ckx_ast_translation_unit()
{
    for (auto it = stmts.begin(); it != stmts.end(); ++it)
    {
        delete *it;
    }

    delete global_table;
}

void
ckx_ast_translation_unit::add_new_stmt(ckx_ast_stmt *_stmt)
{
    stmts.push_back(_stmt);
}

ckx_ast_stmt::ckx_ast_stmt(saber_ptr<ckx_token> _at_token) :
    ckx_ast_node(_at_token)
{}

ckx_ast_stmt::~ckx_ast_stmt() {}

ckx_ast_compound_stmt::ckx_ast_compound_stmt(saber_ptr<ckx_token> _at_token) :
    ckx_ast_stmt(_at_token)
{}

ckx_ast_compound_stmt::~ckx_ast_compound_stmt()
{
    for (ckx_ast_stmt* x : stmts)
        delete x;
}

void
ckx_ast_compound_stmt::add_new_stmt(ckx_ast_stmt* _stmt)
{
    stmts.push_back(_stmt);
}

ckx_ast_if_stmt::ckx_ast_if_stmt(saber_ptr<ckx_token> _at_token,
                                 ckx_ast_expr *_condition,
                                 ckx_ast_stmt *_then_clause,
                                 ckx_ast_stmt *_else_clause) :
    ckx_ast_stmt(_at_token),
    condition(_condition),
    then_clause(_then_clause),
    else_clause(_else_clause)
{}

ckx_ast_if_stmt::~ckx_ast_if_stmt()
{
    delete condition;
    delete then_clause;
    delete else_clause;
}

ckx_ast_while_stmt::ckx_ast_while_stmt(saber_ptr<ckx_token> _at_token,
                                       ckx_ast_expr *_condition,
                                       ckx_ast_stmt *_clause) :
    ckx_ast_stmt(_at_token),
    condition(_condition),
    clause(_clause)
{}

ckx_ast_while_stmt::~ckx_ast_while_stmt()
{
    delete condition;
    delete clause;
}

ckx_ast_do_while_stmt::ckx_ast_do_while_stmt(saber_ptr<ckx_token> _at_token,
                                             ckx_ast_expr *_condition,
                                             ckx_ast_stmt *_clause) :
    ckx_ast_stmt(_at_token),
    condition(_condition),
    clause(_clause)
{}

ckx_ast_do_while_stmt::~ckx_ast_do_while_stmt()
{
    delete condition;
    delete clause;
}

ckx_ast_for_stmt::ckx_ast_for_stmt(saber_ptr<ckx_token> _at_token,
                                   ckx_ast_expr *_init,
                                   ckx_ast_expr *_condition,
                                   ckx_ast_expr *_incr,
                                   ckx_ast_stmt *_clause) :
    ckx_ast_stmt(_at_token),
    init(_init),
    condition(_condition),
    incr(_incr),
    clause(_clause)
{}

ckx_ast_for_stmt::~ckx_ast_for_stmt()
{
    delete init;
    delete condition;
    delete incr;
    delete clause;
}


ckx_ast_break_stmt::ckx_ast_break_stmt(saber_ptr<ckx_token> _at_token) :
    ckx_ast_stmt(_at_token)
{}


ckx_ast_continue_stmt::ckx_ast_continue_stmt(saber_ptr<ckx_token> _at_token) :
    ckx_ast_stmt(_at_token)
{}


ckx_ast_return_stmt::ckx_ast_return_stmt(saber_ptr<ckx_token> _at_token,
                                         ckx_ast_expr *_return_expr) :
    ckx_ast_stmt(_at_token),
    return_expr(_return_expr)
{}

ckx_ast_return_stmt::~ckx_ast_return_stmt()
{
    delete return_expr;
}

ckx_ast_decl_stmt::init_decl::~init_decl() { delete init; }

ckx_ast_decl_stmt::ckx_ast_decl_stmt(saber_ptr<ckx_token> _at_token,
        saber_ptr<ckx_type> _type,
        saber::vector<init_decl> &&_decls) :
    ckx_ast_stmt(_at_token),
    type(_type),
    decls(saber::move(_decls))
{}


ckx_ast_expr_stmt::ckx_ast_expr_stmt(saber_ptr<ckx_token> _at_token,
                                     ckx_ast_expr *_expr) :
    ckx_ast_stmt(_at_token),
    expr(_expr)
{}

ckx_ast_expr_stmt::~ckx_ast_expr_stmt()
{
    delete expr;
}



ckx_ast_func_stmt::ckx_ast_func_stmt(
        saber_ptr<ckx_token> _at_token,
        saber_string_view _name,
        saber::vector<ckx_ast_func_stmt::param_decl> &&_param_decls,
        saber_ptr<ckx_type> _ret_type,
        ckx_ast_compound_stmt *_fnbody) :
    ckx_ast_stmt(_at_token),
    name(_name),
    param_decls(saber::move(_param_decls)),
    ret_type(_ret_type),
    fnbody(_fnbody)
{}

ckx_ast_func_stmt::~ckx_ast_func_stmt()
{
    delete fnbody;
}



ckx_ast_struct_stmt::ckx_ast_struct_stmt(saber_ptr<ckx_token> _at_token,
                                         saber_string_view _name,
                                         saber::vector<field> &&_fields) :
    ckx_ast_stmt(_at_token),
    name(_name),
    fields(saber::move(_fields))
{}

ckx_ast_struct_stmt::~ckx_ast_struct_stmt() {}

const saber::vector<ckx_ast_struct_stmt::field>&
ckx_ast_struct_stmt::get_fields() const
{
    return fields;
}


ckx_ast_variant_stmt::ckx_ast_variant_stmt(saber_ptr<ckx_token> _at_token,
                                           saber_string_view _name,
                                           saber::vector<field> &&_fields) :
    ckx_ast_stmt(_at_token),
    name(_name),
    fields(saber::move(_fields))
{}

ckx_ast_variant_stmt::~ckx_ast_variant_stmt() {}

const saber::vector<ckx_ast_variant_stmt::field>&
ckx_ast_variant_stmt::get_fields() const
{
    return fields;
}


ckx_ast_alias_stmt::ckx_ast_alias_stmt(saber_ptr<ckx_token> _at_token,
                                       saber_string_view _name,
                                       saber_ptr<ckx_type> _type) :
    ckx_ast_stmt(_at_token),
    name(_name),
    type(_type)
{}


ckx_ast_enum_stmt::ckx_ast_enum_stmt(saber_ptr<ckx_token> _at_token,
                                     saber_string_view _name,
                                     saber::vector<enumerator> &&_enumerators) :
    ckx_ast_stmt(_at_token),
    name(_name),
    enumerators(_enumerators)
{}

ckx_ast_enum_stmt::~ckx_ast_enum_stmt() {}

const saber::vector<ckx_ast_enum_stmt::enumerator>&
ckx_ast_enum_stmt::get_enumerators() const
{
    return enumerators;
}


ckx_ast_expr::ckx_ast_expr(saber_ptr<ckx_token> _at_token) :
    ckx_ast_node(_at_token)
{}

ckx_ast_expr::~ckx_ast_expr() {}

ckx_ast_binary_expr::ckx_ast_binary_expr(saber_ptr<ckx_token> _at_token,
                                         ckx_op _opercode,
                                         ckx_ast_expr *_loperand,
                                         ckx_ast_expr *_roperand) :
    ckx_ast_expr(_at_token),
    opercode(_opercode),
    loperand(_loperand),
    roperand(_roperand)
{}

ckx_ast_binary_expr::~ckx_ast_binary_expr()
{
    delete loperand;
    delete roperand;
}

ckx_ast_unary_expr::ckx_ast_unary_expr(saber_ptr<ckx_token> _at_token,
                                       ckx_op _opercode,
                                       ckx_ast_expr *_operand) :
    ckx_ast_expr(_at_token),
    opercode(_opercode),
    operand(_operand)
{}

ckx_ast_unary_expr::~ckx_ast_unary_expr()
{
    delete operand;
}

ckx_ast_subscript_expr::ckx_ast_subscript_expr(saber_ptr<ckx_token> _at_token,
                                               ckx_ast_expr *_base,
                                               ckx_ast_expr *_subscript) :
    ckx_ast_expr(_at_token),
    base(_base),
    subscript(_subscript)
{}

ckx_ast_subscript_expr::~ckx_ast_subscript_expr()
{
    delete base;
    delete subscript;
}

ckx_ast_invoke_expr::ckx_ast_invoke_expr(saber_ptr<ckx_token> _at_token,
                                         ckx_ast_expr *_invokable,
                                         saber::vector<ckx_ast_expr*> &&_args) :
    ckx_ast_expr(_at_token),
    invokable(_invokable),
    args(saber::move(_args))
{
}

ckx_ast_invoke_expr::~ckx_ast_invoke_expr()
{
    delete invokable;
    for (auto& arg : args) delete arg;
}

ckx_ast_extract_expr::ckx_ast_extract_expr(saber_ptr<ckx_token> _at_token,
                                           ckx_ast_expr *_extracted,
                                           saber_string_view _field_name) :
    ckx_ast_expr(_at_token),
    extracted(_extracted),
    field_name(_field_name)
{}

ckx_ast_extract_expr::~ckx_ast_extract_expr()
{
    delete extracted;
}

ckx_ast_enumerator_expr::ckx_ast_enumerator_expr(
        saber_ptr<ckx_token> _at_token,
        saber_string_view _enum_name,
        saber_string_view _enumerator_name) :
    ckx_ast_expr(_at_token),
    enum_name(_enum_name),
    enumerator_name(_enumerator_name)
{}

ckx_ast_cond_expr::ckx_ast_cond_expr(saber_ptr<ckx_token> _at_token,
                                     ckx_ast_expr *_cond_expr,
                                     ckx_ast_expr *_then_expr,
                                     ckx_ast_expr *_else_expr) :
    ckx_ast_expr(_at_token),
    cond_expr(_cond_expr),
    then_expr(_then_expr),
    else_expr(_else_expr)
{}

ckx_ast_cond_expr::~ckx_ast_cond_expr()
{
    delete cond_expr;
    delete then_expr;
    delete else_expr;
}

ckx_ast_id_expr::ckx_ast_id_expr(saber_ptr<ckx_token> _at_token,
                                 saber_string_view _name) :
    ckx_ast_expr(_at_token),
    name(_name)
{}

ckx_ast_id_expr::~ckx_ast_id_expr()
{}

ckx_ast_cast_expr::ckx_ast_cast_expr(saber_ptr<ckx_token> _at_token,
                                     castop _op,
                                     saber_ptr<ckx_type> _desired_type,
                                     ckx_ast_expr *_expr) :
    ckx_ast_expr(_at_token),
    op(_op),
    desired_type(_desired_type),
    expr(_expr)
{}

ckx_ast_cast_expr::~ckx_ast_cast_expr()
{
    delete expr;
}

ckx_ast_sizeof_expr::ckx_ast_sizeof_expr(saber_ptr<ckx_token> _at_token,
                                         saber_ptr<ckx_type> _type) :
    ckx_ast_expr(_at_token),
    type(_type)
{}

ckx_ast_vi_literal_expr::ckx_ast_vi_literal_expr(saber_ptr<ckx_token> _at_token,
                                                 qint64 _val) :
    ckx_ast_expr(_at_token),
    val(_val)
{}

ckx_ast_vr_literal_expr::ckx_ast_vr_literal_expr(saber_ptr<ckx_token> _at_token,
                                                 qreal _val) :
    ckx_ast_expr(_at_token),
    val(_val)
{}

ckx_ast_array_expr::ckx_ast_array_expr(saber_ptr<ckx_token> _at_token,
                                       saber_ptr<ckx_type> _array_of_type) :
    ckx_ast_expr(_at_token),
    array_of_type(_array_of_type)
{}

ckx_ast_array_expr::~ckx_ast_array_expr()
{
    delete start;
    delete finish;
    for (auto &item : init_list)
        delete item;
}

void ckx_ast_array_expr::set_size(qint32 _size)
{
    size = _size;
}

void ckx_ast_array_expr::set_range(ckx_ast_expr *_start, ckx_ast_expr *_finish)
{
    start = _start;
    finish = _finish;
}

void ckx_ast_array_expr::set_init_list(
        saber::vector<ckx_ast_expr *> &&_init_list)
{
    init_list = saber::move(_init_list);
}

} // namespace ckx
