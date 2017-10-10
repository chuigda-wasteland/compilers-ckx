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


#include "ckx_parser_impl.hpp"

namespace ckx
{

template <typename CkxTokenStream>
ckx_parser<CkxTokenStream>::parse_result::parse_result(
        ckx_ast_translation_unit *_trans_unit,
        saber::list<ckx_error*> *_error_list,
        saber::list<ckx_error*> *_warn_list) :
    trans_unit(_trans_unit),
    error_list(_error_list),
    warn_list(_warn_list)
{}

template <typename CkxTokenStream>
ckx_parser<CkxTokenStream>::parse_result::~parse_result()
{
    /// @todo
    /// Note that we are still not sure that who's about to release resources.
    /// We may add several lines of codes here.
}

template <typename CkxTokenStream>
ckx_parser<CkxTokenStream>::parse_result::parse_result(
        parse_result &&_another) :
    trans_unit(_another.trans_unit),
    error_list(_another.error_list),
    warn_list(_another.warn_list)
{
    _another.trans_unit = nullptr;
    _another.error_list = nullptr;
    _another.warn_list = nullptr;
}



template <typename CkxTokenStream>
ckx_parser<CkxTokenStream>::ckx_parser()
{
    p_impl = new detail::ckx_parser_impl<CkxTokenStream>;
}

template <typename CkxTokenStream>
ckx_parser<CkxTokenStream>::~ckx_parser()
{
    delete p_impl;
}

template <typename CkxTokenStream>
typename ckx_parser<CkxTokenStream>::parse_result
ckx_parser<CkxTokenStream>::parse(saber_ptr<CkxTokenStream> _token_stream)
{
    return p_impl->parse_impl(_token_stream);
}

template class ckx_parser<ckx_default_token_stream>;

namespace detail
{

template <typename CkxTokenStream>
typename ckx_parser<CkxTokenStream>::parse_result
ckx_parser_impl<CkxTokenStream>::parse_impl(
        saber_ptr<CkxTokenStream> _token_stream)
{
    token_stream = _token_stream;
    error_list = new saber::list<ckx_error*>;
    warn_list = new saber::list<ckx_error*>;
    typename_table = new ckx_typename_table;

    ckx_ast_translation_unit *trans_unit =
         new ckx_ast_translation_unit(current_token());

    while (current_token()->token_type != ckx_token::type::tk_eoi)
    {
        trans_unit->add_new_stmt(parse_global_stmt());
    }

    typename ckx_parser<CkxTokenStream>::parse_result ret =
         typename ckx_parser<CkxTokenStream>::parse_result(
             trans_unit, error_list, warn_list);
    trans_unit = nullptr;
    warn_list = nullptr;
    delete typename_table;
    return ret;
}

template <typename CkxTokenStream>
ckx_ast_stmt*
ckx_parser_impl<CkxTokenStream>::parse_global_stmt()
{
    switch ( current_token()->token_type )
    {
    case ckx_token::type::tk_vi8:
    case ckx_token::type::tk_vi16:
    case ckx_token::type::tk_vi32:
    case ckx_token::type::tk_vi64:
    case ckx_token::type::tk_vu8:
    case ckx_token::type::tk_vu16:
    case ckx_token::type::tk_vu32:
    case ckx_token::type::tk_vu64:
    case ckx_token::type::tk_vch:
    case ckx_token::type::tk_vr32:
    case ckx_token::type::tk_vr64:
        return parse_decl_stmt();

    /// @attention
    /// Note that currently it is impossible for enum-type :: enumerator
    /// to appear in global statements.
    case ckx_token::type::tk_id:
        if ( is_typename(current_token()) )
            return parse_decl_stmt();
        else
            goto parse_decl_with_id_typename_failed;

    case ckx_token::type::tk_function:
        return parse_func_stmt();

    case ckx_token::type::tk_struct:
        return parse_record_stmt<ckx_ast_struct_stmt>();

    case ckx_token::type::tk_variant:
        return parse_record_stmt<ckx_ast_variant_stmt>();

    case ckx_token::type::tk_enum:
        return parse_enum_stmt();

    case ckx_token::type::tk_alias:
        return parse_alias_stmt();

    case ckx_token::type::tk_ckx:
        return parse_ckx_block();

    /// @todo Module manage system still designing.
    /// We will have C-like preprocessors before creating a powerful
    /// module manage system.
    Q_ON_HOLD(case ckx_token::type::tk_import:)
    Q_ON_HOLD(case ckx_token::type::tk_export:)

    default:
    parse_decl_with_id_typename_failed:
        syntax_error(
            "Expected : typename, 'fn', 'struct', 'variant' or 'enum'"
            " as the commemce of global declaration.",
            current_token()->position);
        return nullptr;
    }
}

template <typename CkxTokenStream>
ckx_ast_stmt*
ckx_parser_impl<CkxTokenStream>::parse_stmt()
{
    switch ( current_token()->token_type )
    {
    case ckx_token::type::tk_vi8:
    case ckx_token::type::tk_vi16:
    case ckx_token::type::tk_vi32:
    case ckx_token::type::tk_vi64:
    case ckx_token::type::tk_vu8:
    case ckx_token::type::tk_vu16:
    case ckx_token::type::tk_vu32:
    case ckx_token::type::tk_vu64:
    case ckx_token::type::tk_vch:
    case ckx_token::type::tk_vr32:
    case ckx_token::type::tk_vr64:
        return parse_decl_stmt();

    case ckx_token::type::tk_id:
        if ( is_typename(current_token()) )
        {
            if (peek_next_token()->token_type == ckx_token::type::tk_scope)
                /// @note we just assume that type is an enum
                return parse_expr_stmt();
            return parse_decl_stmt();
        }
        else
        {
            return parse_expr_stmt();
        }

    case ckx_token::type::tk_add:
    case ckx_token::type::tk_sub:
    case ckx_token::type::tk_mul:
    case ckx_token::type::tk_bit_and:
    case ckx_token::type::tk_bit_not:
    case ckx_token::type::tk_logic_not:

    case ckx_token::type::tk_static_cast:
    case ckx_token::type::tk_reinterpret_cast:
    case ckx_token::type::tk_const_cast:
    case ckx_token::type::tk_ckx_cast:
        return parse_expr_stmt();

    case ckx_token::type::tk_if:        return parse_if_stmt();
    case ckx_token::type::tk_while:     return parse_while_stmt();
    case ckx_token::type::tk_do:        return parse_do_while_stmt();
    case ckx_token::type::tk_for:       return parse_for_stmt();
    case ckx_token::type::tk_break:     return parse_break_stmt();
    case ckx_token::type::tk_continue:  return parse_continue_stmt();
    case ckx_token::type::tk_return:    return parse_return_stmt();
    case ckx_token::type::tk_lbrace:    return parse_compound_stmt();

    case ckx_token::type::tk_semicolon:
        syntax_warn("Empty declaration", current_token()->position);
        next_token();
        break;

    default:
        syntax_error("Ill-formed statement", current_token()->position);
        return nullptr;
    }
    return nullptr;
}



template <typename CkxTokenStream>
ckx_ast_expr_stmt*
ckx_parser_impl<CkxTokenStream>::parse_expr_stmt()
{
    saber_ptr<ckx_token> at_token = current_token();
    ckx_ast_expr_stmt *ret = new ckx_ast_expr_stmt(at_token, parse_expr());
    expect_n_eat(ckx_token::type::tk_semicolon);
    return ret;
}

template <typename CkxTokenStream>
ckx_ast_decl_stmt*
ckx_parser_impl<CkxTokenStream>::parse_decl_stmt()
{
    saber_ptr<ckx_type> type = parse_type();
    ckx_ast_decl_stmt* ret = new ckx_ast_decl_stmt(current_token(), type);

    while (1)
    {
        saber_ptr<ckx_token> token = current_token();
        ckx_ast_expr *init = nullptr;
        next_token();
        if ( current_token()->token_type == ckx_token::type::tk_assign )
        {
            next_token();
            init = parse_expr();
        }

        ret->add_decl(new ckx_ast_init_decl(token, type, token->str, init));

        if ( current_token()->token_type == ckx_token::type::tk_semicolon )
            break;
        expect_n_eat(ckx_token::type::tk_comma);
    }

    expect_n_eat(ckx_token::type::tk_semicolon);
    return ret;
}

template <typename CkxTokenStream>
ckx_ast_func_stmt*
ckx_parser_impl<CkxTokenStream>::parse_func_stmt()
{
    assert(current_token()->token_type == ckx_token::type::tk_function);

    saber_ptr<ckx_token> at_token = current_token();
    next_token();

    saber_string_view func_name = current_token()->str;
    next_token();

    expect_n_eat(ckx_token::type::tk_lparth);
    saber::vector<saber_ptr<ckx_type>> param_type_list;
    saber::vector<ckx_ast_init_decl*> param_decl_list;
    while (1)
    {
        saber_ptr<ckx_token> param_at_token = current_token();
        saber_ptr<ckx_type> param_type = parse_type();
        saber_string_view param_name = current_token()->str;
        next_token();
        param_type_list.push_back(param_type);
        param_decl_list.push_back(
            new ckx_ast_init_decl(
                param_at_token, param_type, param_name, nullptr));

        if (current_token()->token_type == ckx_token::type::tk_comma)
        {
            next_token();
            continue;
        }

        if (current_token()->token_type == ckx_token::type::tk_rparth)
            break;
    }
    expect_n_eat(ckx_token::type::tk_rparth);
    expect_n_eat(ckx_token::type::tk_colon);
    saber_ptr<ckx_type> ret_type = parse_type();

    ckx_ast_compound_stmt *fnbody = nullptr;
    if (current_token()->token_type == ckx_token::type::tk_lbrace)
        fnbody = parse_compound_stmt();
    else
        expect_n_eat(ckx_token::type::tk_semicolon);

    return new ckx_ast_func_stmt(
        at_token, func_name, saber::move(param_decl_list), ret_type, fnbody);
}

template <typename CkxTokenStream>
template <typename CkxAstRecordStmt>
CkxAstRecordStmt*
ckx_parser_impl<CkxTokenStream>::parse_record_stmt()
{
    assert(current_token()->token_type == ckx_token::type::tk_struct \
           || current_token()->token_type == ckx_token::type::tk_variant);
    static_assert(
        saber::traits::type_equivalent<CkxAstRecordStmt, ckx_ast_struct_stmt>::
                value ||
        saber::traits::type_equivalent<CkxAstRecordStmt, ckx_ast_variant_stmt>::
                value,
        "What the fuck!");

    saber_ptr<ckx_token> at_token = current_token();
    next_token();

    saber_string_view record_name = current_token()->str;
    next_token();
    expect_n_eat(ckx_token::type::tk_lbrace);

    CkxAstRecordStmt *ret = new CkxAstRecordStmt(at_token, record_name);

    while (1)
    {
        saber_ptr<ckx_type> decl_type = parse_type();
        while (1)
        {
            saber_string_view dclr_name = current_token()->str;
            ret->add_field(decl_type, dclr_name);
            next_token();

            if (current_token()->token_type == ckx_token::type::tk_comma)
                { next_token(); continue; }

            if (current_token()->token_type == ckx_token::type::tk_semicolon)
                { next_token(); break; }
        }

        if ( current_token()->token_type == ckx_token::type::tk_rbrace )
            break;
    }
    expect_n_eat(ckx_token::type::tk_rbrace);

    typename_table->add_typename(record_name);
    return ret;
}

template <typename CkxTokenStream>
ckx_ast_enum_stmt*
ckx_parser_impl<CkxTokenStream>::parse_enum_stmt()
{
    assert(current_token()->token_type == ckx_token::type::tk_enum);
    saber_ptr<ckx_token> at_token = current_token();
    next_token();
    saber_string_view enum_name = current_token()->str;
    ckx_ast_enum_stmt *ret = new ckx_ast_enum_stmt(at_token, enum_name);

    next_token();
    expect_n_eat(ckx_token::type::tk_lbrace);

    while (1)
    {
        saber_string_view enumerator_name = current_token()->str;
        next_token();
        expect_n_eat(ckx_token::type::tk_assign);
        qint64 enumerator_value = current_token()->v.i64;
        ret->add_enumerator(enumerator_name, enumerator_value);
        next_token();

        if (current_token()->token_type == ckx_token::type::tk_comma)
            { next_token(); continue; }

        if (current_token()->token_type == ckx_token::type::tk_rbrace)
            { next_token(); break; }
    }
    expect_n_eat(ckx_token::type::tk_rbrace);

    typename_table->add_typename(enum_name);
    return ret;
}

template<typename CkxTokenStream>
ckx_ast_alias_stmt *ckx_parser_impl<CkxTokenStream>::parse_alias_stmt()
{
    assert(current_token()->token_type == ckx_token::type::tk_alias);
    saber_ptr<ckx_token> at_token = current_token();
    next_token();

    expect(ckx_token::type::tk_id);
    saber_string_view name = current_token()->str;
    next_token();
    expect_n_eat(ckx_token::type::tk_assign);
    saber_ptr<ckx_type> type = parse_type();
    expect_n_eat(ckx_token::type::tk_semicolon);

    typename_table->add_typename(name);
    return new ckx_ast_alias_stmt(at_token, name, type);
}

template <typename CkxTokenStream>
ckx_ast_compound_stmt*
ckx_parser_impl<CkxTokenStream>::parse_ckx_block()
{
    assert(current_token()->token_type == ckx_token::type::tk_ckx);
    saber_ptr<ckx_token> at_token = current_token();
    next_token();

    expect(ckx_token::type::tk_lbrace);
    return parse_compound_stmt();
}

template <typename CkxTokenStream>
ckx_ast_if_stmt*
ckx_parser_impl<CkxTokenStream>::parse_if_stmt()
{
    assert(current_token()->token_type == ckx_token::type::tk_if);
    saber_ptr<ckx_token> at_token = current_token();
    next_token();

    expect_n_eat(ckx_token::type::tk_lparth);
    ckx_ast_expr *condition = parse_expr();
    expect_n_eat(ckx_token::type::tk_rparth);

    ckx_ast_stmt *then_clause = parse_stmt();
    ckx_ast_stmt *else_clause = nullptr;

    if (current_token()->token_type == ckx_token::type::tk_else)
    {
        next_token();
        else_clause = parse_stmt();
    }

    return new ckx_ast_if_stmt(at_token, condition, then_clause, else_clause);
}

template <typename CkxTokenStream>
ckx_ast_while_stmt*
ckx_parser_impl<CkxTokenStream>::parse_while_stmt()
{
    assert(current_token()->token_type == ckx_token::type::tk_while);
    saber_ptr<ckx_token> at_token = current_token();
    next_token();

    expect_n_eat(ckx_token::type::tk_lparth);
    ckx_ast_expr *condition = parse_expr();
    expect_n_eat(ckx_token::type::tk_rparth);
    ckx_ast_stmt *clause = parse_stmt();

    return new ckx_ast_while_stmt(at_token, condition, clause);
}

template <typename CkxTokenStream>
ckx_ast_do_while_stmt*
ckx_parser_impl<CkxTokenStream>::parse_do_while_stmt()
{
    assert(current_token()->token_type == ckx_token::type::tk_do);
    saber_ptr<ckx_token> at_token = current_token();
    next_token();

    ckx_ast_stmt *clause = parse_stmt();
    expect_n_eat(ckx_token::type::tk_while);
    expect_n_eat(ckx_token::type::tk_lparth);
    ckx_ast_expr *condition = parse_expr();
    expect_n_eat(ckx_token::type::tk_rparth);
    expect_n_eat(ckx_token::type::tk_semicolon);

    return new ckx_ast_do_while_stmt(at_token, condition, clause);
}

template<typename CkxTokenStream>
ckx_ast_for_stmt*
ckx_parser_impl<CkxTokenStream>::parse_for_stmt()
{
    assert(current_token()->token_type == ckx_token::type::tk_for);
    saber_ptr<ckx_token> at_token = current_token();
    next_token();

    expect_n_eat(ckx_token::type::tk_lparth);
    ckx_ast_expr *init = nullptr;
    ckx_ast_expr *cond = nullptr;
    ckx_ast_expr *incr = nullptr;
    if (current_token()->token_type != ckx_token::type::tk_semicolon)
        init = parse_expr();
    expect_n_eat(ckx_token::type::tk_semicolon);
    if (current_token()->token_type != ckx_token::type::tk_semicolon)
        cond = parse_expr();
    expect_n_eat(ckx_token::type::tk_semicolon);
    if (current_token()->token_type != ckx_token::type::tk_semicolon)
        incr = parse_expr();
    expect_n_eat(ckx_token::type::tk_semicolon);
    expect_n_eat(ckx_token::type::tk_rparth);

    ckx_ast_stmt *clause = parse_stmt();
    return new ckx_ast_for_stmt(at_token, init, cond, incr, clause);
}

template <typename CkxTokenStream>
ckx_ast_break_stmt*
ckx_parser_impl<CkxTokenStream>::parse_break_stmt()
{
    assert(current_token()->token_type == ckx_token::type::tk_break);
    saber_ptr<ckx_token> at_token = current_token();
    next_token();
    expect_n_eat(ckx_token::type::tk_semicolon);

    return new ckx_ast_break_stmt(at_token);
}

template <typename CkxTokenStream>
ckx_ast_continue_stmt*
ckx_parser_impl<CkxTokenStream>::parse_continue_stmt()
{
    assert(current_token()->token_type == ckx_token::type::tk_continue);
    saber_ptr<ckx_token> at_token = current_token();
    next_token();
    expect_n_eat(ckx_token::type::tk_semicolon);

    return new ckx_ast_continue_stmt(at_token);
}

template <typename CkxTokenStream>
ckx_ast_return_stmt*
ckx_parser_impl<CkxTokenStream>::parse_return_stmt()
{
    assert(current_token()->token_type == ckx_token::type::tk_return);
    saber_ptr<ckx_token> at_token = current_token();
    next_token();

    ckx_ast_expr *expr = nullptr;
    if (current_token()->token_type != ckx_token::type::tk_semicolon)
        expr = parse_expr();
    expect_n_eat(ckx_token::type::tk_semicolon);

    return new ckx_ast_return_stmt(at_token, expr);
}

template <typename CkxTokenStream>
ckx_ast_compound_stmt*
ckx_parser_impl<CkxTokenStream>::parse_compound_stmt()
{
    saber_ptr<ckx_token> at_token = current_token();
    expect_n_eat(ckx_token::type::tk_lbrace);
    ckx_ast_compound_stmt *ret = new ckx_ast_compound_stmt(at_token);
    while (current_token()->token_type != ckx_token::type::tk_rbrace)
    {
        ret->add_new_stmt(parse_stmt());
    }
    expect_n_eat(ckx_token::type::tk_rbrace);
    return ret;
}

template <typename CkxTokenStream>
ckx_ast_expr*
ckx_parser_impl<CkxTokenStream>::parse_expr()
{
    return parse_assign_expr();
}

template <typename CkxTokenStream>
ckx_ast_expr*
ckx_parser_impl<CkxTokenStream>::parse_cond_expr()
{
    saber_ptr<ckx_token> at_token = current_token();

    ckx_ast_expr *binary_expr = parse_binary_expr(0);

    if (current_token()->token_type == ckx_token::type::tk_ques)
    {
        next_token();
        ckx_ast_expr *then_expr = parse_expr();
        expect_n_eat(ckx_token::type::tk_colon);
        ckx_ast_expr *else_expr = parse_expr();

        return new ckx_ast_cond_expr(
            at_token, binary_expr, then_expr, else_expr);
    }
    return binary_expr;
}

template <typename CkxTokenStream>
ckx_ast_expr*
ckx_parser_impl<CkxTokenStream>::parse_binary_expr(quint8 _prec)
{
    saber_ptr<ckx_token> at_token = current_token();

    ckx_ast_expr *expr = parse_unary_expr();
    ckx_op op = ckx_op_helper::token2binary(current_token()->token_type);
    quint8 new_prec;
    while (ckx_op_helper::is_binary(op)
           && (new_prec=ckx_op_helper::precedence(op)) >= _prec)
    {
        next_token();

        ckx_ast_expr *binary_expr =
            new ckx_ast_binary_expr(
                 at_token, op, expr, parse_binary_expr(new_prec+1));
        expr = binary_expr;
        op = ckx_op_helper::token2binary(current_token()->token_type);
    }
    return expr;
}

template <typename CkxTokenStream>
ckx_ast_expr*
ckx_parser_impl<CkxTokenStream>::parse_assign_expr()
{
    saber_ptr<ckx_token> at_token = current_token();
    ckx_ast_expr *expr = parse_cond_expr();

    ckx_op current_op =
        ckx_op_helper::token2binary(current_token()->token_type);
    if (ckx_op_helper::is_assign(current_op))
    {
        next_token();
        return new ckx_ast_binary_expr(
            at_token, current_op, expr, parse_assign_expr());
    }
    return expr;
}

template <typename CkxTokenStream>
ckx_ast_cast_expr*
ckx_parser_impl<CkxTokenStream>::parse_cast_expr()
{
    saber_ptr<ckx_token> at_token = current_token();

    ckx_ast_cast_expr::castop castop;
    switch (current_token()->token_type)
    {
    case ckx_token::type::tk_static_cast:
        castop = ckx_ast_cast_expr::castop::cst_static; break;
    case ckx_token::type::tk_const_cast:
        castop = ckx_ast_cast_expr::castop::cst_const; break;
    case ckx_token::type::tk_reinterpret_cast:
        castop = ckx_ast_cast_expr::castop::cst_reinterpret; break;
    case ckx_token::type::tk_ckx_cast:
        castop = ckx_ast_cast_expr::castop::cst_ckx; break;
    default: assert(false); // What the fuck!
    }
    next_token();
    expect_n_eat(ckx_token::type::tk_lt);
    saber_ptr<ckx_type> desired_type = parse_type();
    expect_n_eat(ckx_token::type::tk_gt);
    expect_n_eat(ckx_token::type::tk_lparth);
    ckx_ast_expr *expr = parse_expr();
    expect_n_eat(ckx_token::type::tk_rparth);

    return new ckx_ast_cast_expr(at_token, castop, desired_type, expr);
}

template <typename CkxTokenStream>
ckx_ast_expr*
ckx_parser_impl<CkxTokenStream>::parse_unary_expr()
{
    saber_ptr<ckx_token> at_token = current_token();

    switch (current_token()->token_type)
    {
    case ckx_token::type::tk_static_cast:
    case ckx_token::type::tk_const_cast:
    case ckx_token::type::tk_reinterpret_cast:
    case ckx_token::type::tk_ckx_cast:
        return parse_cast_expr();

    case ckx_token::type::tk_add:
    case ckx_token::type::tk_sub:
    case ckx_token::type::tk_bit_and:
    case ckx_token::type::tk_mul:
    case ckx_token::type::tk_bit_not:
    case ckx_token::type::tk_logic_not:
    case ckx_token::type::tk_inc:
    case ckx_token::type::tk_dec:
        next_token();
        return new ckx_ast_unary_expr(
            at_token, ckx_op_helper::token2unary(at_token->token_type),
            parse_unary_expr());

    default:
        return parse_postfix_expr();
    }
}

template <typename CkxTokenStream>
ckx_ast_expr*
ckx_parser_impl<CkxTokenStream>::parse_postfix_expr()
{
    saber_ptr<ckx_token> at_token = current_token();
    ckx_ast_expr *ret = nullptr;

    /// @todo note that this is a temporary solution.
    /// Disgusting. Remove it as soon as possible.
    if (current_token()->token_type == ckx_token::type::tk_lparth)
    {
        next_token();
        ret = parse_expr();
        expect_n_eat(ckx_token::type::tk_rparth);
        return ret;
    }

    while (1)
    {
        switch (current_token()->token_type)
        {
        case ckx_token::type::tk_id:
            if (peek_next_token()->token_type == ckx_token::type::tk_scope)
                /// @todo add solution for enumerators
                ;
            /// @attention fallthrough

        case ckx_token::type::tk_vi_literal:
        case ckx_token::type::tk_vr_literal:
            {
                ret = parse_basic_expr();
                break;
            }

        case ckx_token::type::tk_lparth:
            {
                next_token();
                saber::vector<ckx_ast_expr*> args;
                while (current_token()->token_type
                       != ckx_token::type::tk_rparth)
                {
                    args.push_back(parse_expr());
                    expect_n_eat(ckx_token::type::tk_comma);
                }
                expect_n_eat(ckx_token::type::tk_rparth);
                ret = new ckx_ast_invoke_expr(at_token, ret, saber::move(args));
                break;
            }

        case ckx_token::type::tk_lbracket:
            {
                next_token();
                ret = new ckx_ast_subscript_expr(at_token, ret, parse_expr());
                expect_n_eat(ckx_token::type::tk_rbracket);
                break;
            }

        case ckx_token::type::tk_dot:
            {
                next_token();
                ret = new ckx_ast_extract_expr(
                          at_token, ret, current_token()->str);
                next_token();
                break;
            }

        default:
            return ret;
        }
    }

    return nullptr;
}

template <typename CkxTokenStream>
ckx_ast_expr*
ckx_parser_impl<CkxTokenStream>::parse_basic_expr()
{
    saber_ptr<ckx_token> at_token = current_token();

    switch (current_token()->token_type)
    {
    case ckx_token::type::tk_vi_literal:
        {
            ckx_ast_expr* ret = new ckx_ast_vi_literal_expr(
                current_token(), current_token()->v.i64);
            next_token();
            return ret;
        }

    case ckx_token::type::tk_vr_literal:
        {
            ckx_ast_expr* ret = new ckx_ast_vr_literal_expr(
                current_token(), current_token()->v.r);
            next_token();
            return ret;
        }

    case ckx_token::type::tk_id:
        {
            saber_string_view name = current_token()->str;
            if (is_typename(current_token()))
            {
                if (peek_next_token()->token_type == ckx_token::type::tk_scope)
                {
                    next_token();
                    next_token();
                    expect(ckx_token::type::tk_id);
                    saber_string_view enumer = current_token()->str;
                    ckx_ast_expr *ret =
                        new ckx_ast_enumerator_expr(at_token, name, enumer);
                    next_token();
                    return ret;
                }
            }
            next_token();
            return new ckx_ast_id_expr(at_token, name);
        }

    case ckx_token::type::tk_lparth:
        {
            next_token();
            ckx_ast_expr *ret = parse_expr();
            expect_n_eat(ckx_token::type::tk_rparth);
            return ret;
        }

    default:
        assert(0);
    }
}

template <typename CkxTokenStream>
saber_ptr<ckx_type>
ckx_parser_impl<CkxTokenStream>::parse_type()
{
    saber_ptr<ckx_type> type;
    if (current_token()->token_type == ckx_token::type::tk_id)
    {
        type = new ckx_id_type(current_token()->str);
    }
    else
    {
        type = ckx_type_helper::get_type( current_token()->token_type );
    }
    next_token();

    while (1)
    {
        switch ( current_token()->token_type )
        {
        case ckx_token::type::tk_const:
            type = ckx_type_helper::qual_const(type); next_token(); break;

        case ckx_token::type::tk_mul:
            type = ckx_type_helper::pointer_to(type); next_token(); break;

        default:
            return type;
        }
    }
}



template <typename CkxTokenStream>
inline saber_ptr<ckx_token>
ckx_parser_impl<CkxTokenStream>::current_token()
{
    return token_stream.get()->operator[](0);
}

template <typename CkxTokenStream>
inline saber_ptr<ckx_token>
ckx_parser_impl<CkxTokenStream>::peek_next_token()
{
    return token_stream.get()->operator[](1);
}

template <typename CkxTokenStream>
inline void
ckx_parser_impl<CkxTokenStream>::next_token()
{
    return token_stream.get()->operator++();
}

template <typename CkxTokenStream>
inline void
ckx_parser_impl<CkxTokenStream>::expect_n_eat(ckx_token::type _token_type)
{
    if (current_token()->token_type == _token_type)
        next_token();

    syntax_error("Unexpected token", current_token()->position);
}

template <typename CkxTokenStream>
inline void
ckx_parser_impl<CkxTokenStream>::expect(ckx_token::type _token_type)
{
    if (current_token()->token_type == _token_type)
        return;
    syntax_error("Unexpected token", current_token()->position);
}

template <typename CkxTokenStream>
bool
ckx_parser_impl<CkxTokenStream>::is_typename(saber_ptr<ckx_token> _token)
{
    assert(_token->token_type == ckx_token::type::tk_id);
    return typename_table->query_typename(_token->str);
}



template <typename CkxTokenStream>
void
ckx_parser_impl<CkxTokenStream>::syntax_error(saber_string &&_reason,
                                              const qcoord &_pos)
{
    /// @todo finalize this error processing system.
    Q_UNUSED(_reason);
    Q_UNUSED(_pos);
}

template <typename CkxTokenStream>
void
ckx_parser_impl<CkxTokenStream>::syntax_warn(saber_string&& _reason,
                                             const qcoord& _pos)
{
    /// @todo finalize this error processing system.
    Q_UNUSED(_reason);
    Q_UNUSED(_pos);
}


template <typename CkxTokenStream>
void
ckx_parser_impl<CkxTokenStream>::skip2_token(const ckx_token_set& _token_set)
{
    while ( _token_set.find(current_token()->token_type) == _token_set.end()
            && (current_token()->token_type != ckx_token::type::tk_eoi) )
        next_token();
}

} // namespace detail

} // namespace ckx
