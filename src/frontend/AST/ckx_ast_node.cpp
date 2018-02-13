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


#include "frontend/AST/ckx_ast_node.hpp"

namespace ckx
{

ckx_ast_translation_unit::ckx_ast_translation_unit() {}

ckx_ast_translation_unit::~ckx_ast_translation_unit()
{
    for (auto it = stmts.begin(); it != stmts.end(); ++it)
        delete *it;
}

void
ckx_ast_translation_unit::add_new_stmt(ckx_ast_stmt *_stmt)
{
    stmts.push_back(_stmt);
}


ckx_ast_compound_stmt::ckx_ast_compound_stmt(
        ckx_src_rng _lbrace_rng,
        ckx_src_rng _rbrace_rng,
        saber::vector<ckx_ast_stmt *> &&_stmts) :
    lbrace_rng(_lbrace_rng),
    rbrace_rng(_rbrace_rng),
    stmts(saber::move(_stmts)) {}

ckx_ast_compound_stmt::~ckx_ast_compound_stmt()
{
    for (ckx_ast_stmt* x : stmts)
        delete x;
}


ckx_ast_if_stmt::ckx_ast_if_stmt(ckx_src_rng _if_rng,
                                 ckx_src_rng _else_rng,
                                 ckx_ast_expr *_condition,
                                 ckx_ast_stmt *_then_clause,
                                 ckx_ast_stmt *_else_clause) :
    if_rng(_if_rng), else_rng(_else_rng),
    condition(_condition),
    then_clause(_then_clause),
    else_clause(_else_clause) {}

ckx_ast_if_stmt::ckx_ast_if_stmt(ckx_src_rng _if_rng,
                                 ckx_ast_expr *_condition,
                                 ckx_ast_stmt *_then_clause) :
    if_rng(_if_rng),
    else_rng(ckx_src_rng(-1, -1, -1, -1)),
    condition(_condition),
    then_clause(_then_clause),
    else_clause(nullptr) {}

ckx_ast_if_stmt::~ckx_ast_if_stmt()
{
    delete condition;
    delete then_clause;
    delete else_clause;
}

ckx_ast_while_stmt::ckx_ast_while_stmt(ckx_src_rng _while_rng,
                                       ckx_ast_expr *_condition,
                                       ckx_ast_stmt *_clause) :
    while_rng(_while_rng),
    condition(_condition),
    clause(_clause) {}

ckx_ast_while_stmt::~ckx_ast_while_stmt()
{
    delete condition;
    delete clause;
}

ckx_ast_do_while_stmt::ckx_ast_do_while_stmt(ckx_src_rng _do_rng,
                                             ckx_src_rng _while_rng,
                                             ckx_ast_expr *_condition,
                                             ckx_ast_stmt *_clause) :
    do_rng(_do_rng),
    while_rng(_while_rng),
    condition(_condition),
    clause(_clause)
{}

ckx_ast_do_while_stmt::~ckx_ast_do_while_stmt()
{
    delete condition;
    delete clause;
}

ckx_ast_for_stmt::ckx_ast_for_stmt(ckx_src_rng _for_rng,
                                   ckx_ast_expr *_init,
                                   ckx_ast_expr *_condition,
                                   ckx_ast_expr *_incr,
                                   ckx_ast_stmt *_clause) :
    for_rng(_for_rng),
    init(_init), condition(_condition), incr(_incr), clause(_clause) {}

ckx_ast_for_stmt::~ckx_ast_for_stmt()
{
    delete init;
    delete condition;
    delete incr;
    delete clause;
}


ckx_ast_break_stmt::ckx_ast_break_stmt(ckx_src_rng _rng) : rng(_rng) {}


ckx_ast_continue_stmt::ckx_ast_continue_stmt(ckx_src_rng _rng) : rng(_rng) {}


ckx_ast_return_stmt::ckx_ast_return_stmt(ckx_src_rng _return_rng,
                                         ckx_ast_expr *_return_expr) :
    return_rng(_return_rng),
    return_expr(_return_expr) {}

ckx_ast_return_stmt::~ckx_ast_return_stmt()
{
    delete return_expr;
}

ckx_ast_decl_stmt::init_decl::~init_decl() { delete init; }

ckx_ast_decl_stmt::ckx_ast_decl_stmt(ckx_prelexed_type&& _type,
                                     saber::vector<init_decl>&& _decls) :
    type(saber::move(_type)), decls(saber::move(_decls)) {}


ckx_ast_expr_stmt::ckx_ast_expr_stmt(ckx_ast_expr *_expr) :
    expr(_expr) {}

ckx_ast_expr_stmt::~ckx_ast_expr_stmt()
{
    delete expr;
}



ckx_ast_func_stmt::ckx_ast_func_stmt(
        ckx_src_rng _kwd_rng,
        saber_string_view _name,
        saber::vector<ckx_ast_func_stmt::param_decl> &&_param_decls,
        ckx_prelexed_type _ret_type,
        ckx_ast_compound_stmt *_fnbody) :
    kwd_rng(_kwd_rng),
    name(_name),
    param_decls(saber::move(_param_decls)),
    ret_type(saber::move(_ret_type)),
    fnbody(_fnbody) {}

ckx_ast_func_stmt::~ckx_ast_func_stmt()
{
    delete fnbody;
}



ckx_ast_record_stmt::ckx_ast_record_stmt(ckx_src_rng _kwd_rng,
                                         ckx_src_rng _id_rng,
                                         ckx_src_rng _lbrace_rng,
                                         ckx_src_rng _rbrace_rng,
                                         record_tag _tag,
                                         saber_string_view _name,
                                         saber::vector<field_row> &&_fields) :
    kwd_rng(_kwd_rng), id_rng(_id_rng),
    lbrace_rng(_lbrace_rng), rbrace_rng(_rbrace_rng),
    tag(_tag), name(_name), fields(saber::move(_fields)) {}


ckx_ast_enum_stmt::ckx_ast_enum_stmt(ckx_src_rng _kwd_rng,
                                     ckx_src_rng _id_rng,
                                     ckx_src_rng _lbrace_rng,
                                     ckx_src_rng _rbrace_rng,
                                     saber_string_view _name,
                                     saber::vector<enumerator> &&_enumerators) :
    kwd_rng(_kwd_rng), id_rng(_id_rng),
    lbrace_rng(_lbrace_rng), rbrace_rng(_rbrace_rng),
    name(_name),
    enumerators(_enumerators) {}

ckx_ast_enum_stmt::~ckx_ast_enum_stmt() {}


ckx_ast_alias_stmt::ckx_ast_alias_stmt(ckx_src_rng _kwd_rng,
                                       ckx_src_rng _id_rng,
                                       saber_string_view _name,
                                       ckx_prelexed_type _type) :
    kwd_rng(_kwd_rng), id_rng(_id_rng),
    name(_name), type(saber::move(_type)) {}


ckx_ast_binary_expr::ckx_ast_binary_expr(ckx_src_rng _operator_rng,
                                         ckx_op _opercode,
                                         ckx_ast_expr *_loperand,
                                         ckx_ast_expr *_roperand) :
    operator_rng(_operator_rng),
    opercode(_opercode),
    loperand(_loperand),
    roperand(_roperand) {}

ckx_ast_binary_expr::~ckx_ast_binary_expr()
{
    delete loperand;
    delete roperand;
}

ckx_ast_unary_expr::ckx_ast_unary_expr(ckx_src_rng _operator_rng,
                                       ckx_op _opercode,
                                       ckx_ast_expr *_operand) :
    operator_rng(_operator_rng),
    opercode(_opercode),
    operand(_operand) {}

ckx_ast_unary_expr::~ckx_ast_unary_expr()
{
    delete operand;
}

ckx_ast_subscript_expr::ckx_ast_subscript_expr(ckx_src_rng _lbracket_rng,
                                               ckx_src_rng _rbracket_rng,
                                               ckx_ast_expr *_base,
                                               ckx_ast_expr *_subscript) :
    lbracket_rng(_lbracket_rng),
    rbracket_rng(_rbracket_rng),
    base(_base),
    subscript(_subscript) {}

ckx_ast_subscript_expr::~ckx_ast_subscript_expr()
{
    delete base;
    delete subscript;
}

ckx_ast_invoke_expr::ckx_ast_invoke_expr(ckx_src_rng _lparen_rng,
                                         ckx_src_rng _rparen_rng,
                                         ckx_ast_expr *_invokable,
                                         saber::vector<ckx_ast_expr*> &&_args) :
    lparen_rng(_lparen_rng),
    rparen_rng(_rparen_rng),
    invokable(_invokable),
    args(saber::move(_args)) {}

ckx_ast_invoke_expr::~ckx_ast_invoke_expr()
{
    delete invokable;
    for (auto& arg : args) delete arg;
}

ckx_ast_extract_expr::ckx_ast_extract_expr(ckx_ast_expr *_extracted,
                                           saber_string_view _field_name) :
    extracted(_extracted),
    field_name(_field_name) {}

ckx_ast_extract_expr::~ckx_ast_extract_expr()
{
    delete extracted;
}

ckx_ast_enumerator_expr::ckx_ast_enumerator_expr(
        ckx_src_rng _enum_rng,
        ckx_src_rng _enumerator_rng,
        saber_string_view _enum_name,
        saber_string_view _enumerator_name) :
    enum_rng(_enum_rng),
    enumerator_rng(_enumerator_rng),
    enum_name(_enum_name),
    enumerator_name(_enumerator_name) {}

ckx_ast_cond_expr::ckx_ast_cond_expr(ckx_src_rng _ques_rng,
                                     ckx_src_rng _colon_rng,
                                     ckx_ast_expr *_cond_expr,
                                     ckx_ast_expr *_then_expr,
                                     ckx_ast_expr *_else_expr) :
    ques_rng(_ques_rng),
    colon_rng(_colon_rng),
    cond_expr(_cond_expr),
    then_expr(_then_expr),
    else_expr(_else_expr) {}

ckx_ast_cond_expr::~ckx_ast_cond_expr()
{
    delete cond_expr;
    delete then_expr;
    delete else_expr;
}

ckx_ast_id_expr::ckx_ast_id_expr(ckx_src_rng _rng,
                                 saber_string_view _name) :
    rng(_rng), name(_name) {}

ckx_ast_cast_expr::ckx_ast_cast_expr(ckx_src_rng _kwd_rng,
                                     castop _op,
                                     ckx_prelexed_type _desired_type,
                                     ckx_ast_expr *_expr) :
    kwd_rng(_kwd_rng),
    op(_op),
    desired_type(saber::move(_desired_type)),
    expr(_expr) {}

ckx_ast_cast_expr::~ckx_ast_cast_expr()
{
    delete expr;
}

ckx_ast_sizeof_expr::ckx_ast_sizeof_expr(ckx_src_rng _kwd_rng,
                                         ckx_prelexed_type _type) :
    kwd_rng(_kwd_rng), type(saber::move(_type)) {}

ckx_ast_vi_literal_expr::ckx_ast_vi_literal_expr(ckx_src_rng _rng, qint64 _val):
    rng(_rng), val(_val) {}

ckx_ast_vr_literal_expr::ckx_ast_vr_literal_expr(ckx_src_rng _rng, qreal _val) :
    rng(_rng), val(_val) {}

ckx_ast_bool_literal_expr::ckx_ast_bool_literal_expr(ckx_src_rng _rng,
                                                     bool _val) :
    rng(_rng), val(_val) {}

ckx_ast_nullptr_expr::ckx_ast_nullptr_expr(ckx_src_rng _rng) : rng(_rng) {}

ckx_ast_array_expr::ckx_ast_array_expr(ckx_prelexed_type _array_of_type) :
    array_of_type(saber::move(_array_of_type)) {}

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
