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
#include "c8assert.hpp"

namespace ckx
{


ckx_parser::parse_result::parse_result(ckx_ast_translation_unit *_trans_unit,
        saber::list<ckx::ckx_syntax_error> &&_error_list,
        saber::list<ckx::ckx_syntax_error> &&_warn_list) :
    trans_unit(saber::move(_trans_unit)),
    error_list(saber::move(_error_list)),
    warn_list(saber::move(_warn_list))
{}


ckx_parser::parse_result::~parse_result()
{
    /// @todo
    /// Note that we are still not sure that who's about to release resources.
    /// We may add several lines of codes here.
}


ckx_parser::parse_result::parse_result(parse_result &&_another) :
    trans_unit(saber::move(_another.trans_unit)),
    error_list(saber::move(_another.error_list)),
    warn_list(saber::move(_another.warn_list)) {}



ckx_parser::ckx_parser()
{
    p_impl = new detail::ckx_parser_impl;
}

ckx_parser::~ckx_parser()
{
    delete p_impl;
}

typename ckx_parser::parse_result
ckx_parser::parse(ckx_token_stream* _token_stream)
{
    return p_impl->parse_impl(_token_stream);
}



namespace detail
{

typename ckx_parser::parse_result
ckx_parser_impl::parse_impl(ckx_token_stream* _token_stream)
{
    token_stream = _token_stream;
    ckx_ast_translation_unit *trans_unit = new ckx_ast_translation_unit();

    while (current_token().token_type != ckx_token::type::tk_eoi)
        trans_unit->add_new_stmt(parse_global_stmt());

    ckx_parser::parse_result ret =
        ckx_parser::parse_result(
            trans_unit, saber::move(error_list), saber::move(warn_list));

    trans_unit = nullptr;
    error_list.clear();
    warn_list.clear();
    typename_table.cleanup();
    return ret;
}


ckx_ast_stmt*
ckx_parser_impl::parse_global_stmt()
{
    switch ( current_token().token_type )
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
        if ( id_is_typename(current_token()) )
            return parse_decl_stmt();
        else
            goto parse_decl_with_id_typename_failed;

    case ckx_token::type::tk_function:
        return parse_func_stmt();

    case ckx_token::type::tk_struct:
    case ckx_token::type::tk_variant:
        return parse_record_stmt();

    case ckx_token::type::tk_enum:
        return parse_enum_stmt();

    case ckx_token::type::tk_alias:
        return parse_alias_stmt();

    /// @todo Module manage system still designing.
    /// We will have C-like preprocessors before creating a powerful
    /// module manage system.
    Q_ON_HOLD(case ckx_token::type::tk_import:)
    Q_ON_HOLD(case ckx_token::type::tk_export:)

    default:
    parse_decl_with_id_typename_failed:
        syntax_error(current_token().rng,
                     saber_string_pool::create_view(
                     "Expected : typename, 'fn', 'struct', 'variant' or 'enum'"
                     " as the commemce of global declaration."));
        return nullptr;
    }
}


ckx_ast_stmt*
ckx_parser_impl::parse_stmt()
{
    switch ( current_token().token_type )
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
        if ( id_is_typename(current_token()) )
        {
            if (peek_next_token().token_type == ckx_token::type::tk_scope)
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
    case ckx_token::type::tk_inc:
    case ckx_token::type::tk_dec:
    case ckx_token::type::tk_lparen:

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
        syntax_warn(current_token().rng,
                    saber_string_pool::create_view("Empty declaration"));
        next_token();
        break;

    default:
        syntax_error(current_token().rng,
                     saber_string_pool::create_view("Ill-formed statement"));
        return nullptr;
    }
    return nullptr;
}


ckx_ast_expr_stmt*
ckx_parser_impl::parse_expr_stmt()
{
    ckx_ast_expr *expr = parse_expr();
    expect_n_eat(ckx_token::type::tk_semicolon);
    return new ckx_ast_expr_stmt(expr);
}


ckx_ast_decl_stmt*
ckx_parser_impl::parse_decl_stmt()
{
    ckx_prelexed_type type = parse_type();
    saber::vector<ckx_ast_decl_stmt::init_decl> decls;
    while (1)
    {
        ckx_src_rng id_range = current_token().rng;
        saber_string_view id_string = current_token().str;

        ckx_ast_expr *init = nullptr;
        next_token();
        if ( current_token().token_type == ckx_token::type::tk_assign )
        {
            next_token();
            init = parse_init_expr();
        }
        decls.emplace_back(id_range, id_string, init);
        if ( current_token().token_type == ckx_token::type::tk_semicolon )
            break;
        expect_n_eat(ckx_token::type::tk_comma);
    }
    expect_n_eat(ckx_token::type::tk_semicolon);
    return new ckx_ast_decl_stmt(saber::move(type), saber::move(decls));
}


ckx_ast_func_stmt*
ckx_parser_impl::parse_func_stmt()
{
    C8ASSERT(current_token().token_type == ckx_token::type::tk_function);

    ckx_src_rng start_source_range = current_token().rng;
    next_token();

    saber_string_view func_name = current_token().str;
    next_token();

    expect_n_eat(ckx_token::type::tk_lparen);
    saber::vector<ckx_ast_func_stmt::param_decl> param_decl_list;

    while (1)
    {
        ckx_src_rng param_rng = current_token().rng;
        ckx_prelexed_type param_type = parse_type();
        saber_string_view param_name = current_token().str;
        next_token();
        param_decl_list.emplace_back(
            param_rng, saber::move(param_type), param_name);

        if (current_token().token_type == ckx_token::type::tk_comma)
        {
            next_token();
            continue;
        }

        if (current_token().token_type == ckx_token::type::tk_rparen)
            break;
    }
    expect_n_eat(ckx_token::type::tk_rparen);
    expect_n_eat(ckx_token::type::tk_colon);
    ckx_prelexed_type ret_type = parse_type();

    ckx_ast_compound_stmt *fnbody = nullptr;
    if (current_token().token_type == ckx_token::type::tk_lbrace)
        fnbody = parse_compound_stmt();
    else
        expect_n_eat(ckx_token::type::tk_semicolon);

    return new ckx_ast_func_stmt(start_source_range, func_name,
                                 saber::move(param_decl_list),
                                 saber::move(ret_type), fnbody);
}


ckx_ast_record_stmt*
ckx_parser_impl::parse_record_stmt()
{
    C8ASSERT(current_token().token_type == ckx_token::type::tk_struct \
           || current_token().token_type == ckx_token::type::tk_variant);

    ckx_ast_record_stmt::record_tag tag =
        current_token().token_type == ckx_token::type::tk_struct ?
            ckx_ast_record_stmt::record_tag::rt_struct :
            ckx_ast_record_stmt::record_tag::rt_variant;

    ckx_src_rng kwd_rng = current_token().rng;
    next_token();

    ckx_src_rng id_rng = current_token().rng;
    saber_string_view record_name = current_token().str;
    typename_table.add_typename(record_name);

    next_token();
    ckx_src_rng lbrace_rng = expect_n_eat(ckx_token::type::tk_lbrace).get();
    saber::vector<ckx_ast_record_stmt::field_row> field_rows;
    while (1)
    {
        ckx_prelexed_type decl_type = parse_type();
        saber::vector<ckx_ast_record_stmt::field> fields;
        while (1)
        {
            ckx_src_rng dclr_rng = current_token().rng;
            saber_string_view dclr_name = current_token().str;
            fields.emplace_back(dclr_rng, dclr_name);
            next_token();
            if (current_token().token_type == ckx_token::type::tk_comma)
                { next_token(); }
            else
            if (current_token().token_type == ckx_token::type::tk_semicolon)
                { next_token(); break; }
        }
        field_rows.emplace_back(saber::move(decl_type), saber::move(fields));

        if ( current_token().token_type == ckx_token::type::tk_rbrace )
            break;
    }
    ckx_src_rng rbrace_rng = expect_n_eat(ckx_token::type::tk_rbrace).get();
    return new ckx_ast_record_stmt(kwd_rng, id_rng, lbrace_rng, rbrace_rng,
                                   tag, record_name, saber::move(field_rows));
}


ckx_ast_enum_stmt*
ckx_parser_impl::parse_enum_stmt()
{
    C8ASSERT(current_token().token_type == ckx_token::type::tk_enum);
    ckx_src_rng kwd_rng = current_token().rng;
    next_token();

    ckx_src_rng id_rng = current_token().rng;
    saber_string_view enum_name = current_token().str;
    typename_table.add_typename(enum_name);
    next_token();

    ckx_src_rng lbrace_rng = expect_n_eat(ckx_token::type::tk_lbrace).get();

    saber::vector<ckx_ast_enum_stmt::enumerator> enumerators;
    while (1)
    {
        ckx_src_rng id_rng = current_token().rng;
        saber_string_view enumerator_name = current_token().str;
        next_token();
        expect_n_eat(ckx_token::type::tk_assign);
        ckx_src_rng value_rng = current_token().rng;
        qint64 enumerator_value = current_token().v.i64;

        enumerators.emplace_back(id_rng, value_rng,
                                 enumerator_name, enumerator_value);
        next_token();

        if (current_token().token_type == ckx_token::type::tk_comma)
            next_token();

        if (current_token().token_type == ckx_token::type::tk_rbrace)
            break;
    }
    ckx_src_rng rbrace_rng = expect_n_eat(ckx_token::type::tk_rbrace).get();
    return new ckx_ast_enum_stmt(kwd_rng, id_rng, lbrace_rng, rbrace_rng,
                                 enum_name, saber::move(enumerators));
}


ckx_ast_alias_stmt *ckx_parser_impl::parse_alias_stmt()
{
    C8ASSERT(current_token().token_type == ckx_token::type::tk_alias);
    ckx_src_rng kwd_rng = current_token().rng;
    next_token();

    expect(ckx_token::type::tk_id);
    ckx_src_rng id_rng = current_token().rng;
    saber_string_view name = current_token().str;
    next_token();
    expect_n_eat(ckx_token::type::tk_assign);
    ckx_prelexed_type type = parse_type();
    expect_n_eat(ckx_token::type::tk_semicolon);

    typename_table.add_typename(name);
    return new ckx_ast_alias_stmt(kwd_rng, id_rng, name, saber::move(type));
}


ckx_ast_if_stmt*
ckx_parser_impl::parse_if_stmt()
{
    C8ASSERT(current_token().token_type == ckx_token::type::tk_if);
    ckx_src_rng if_rng = current_token().rng;
    next_token();

    expect_n_eat(ckx_token::type::tk_lparen);
    ckx_ast_expr *condition = parse_expr();
    expect_n_eat(ckx_token::type::tk_rparen);

    ckx_ast_stmt *then_clause = parse_stmt();

    if (current_token().token_type == ckx_token::type::tk_else)
    {
        ckx_src_rng else_rng = current_token().rng;
        next_token();
        ckx_ast_stmt *else_clause = parse_stmt();
        return new ckx_ast_if_stmt(
            if_rng, else_rng, condition, then_clause, else_clause);
    }
    else
    {
        return new ckx_ast_if_stmt(if_rng, condition, then_clause);
    }
}


ckx_ast_while_stmt*
ckx_parser_impl::parse_while_stmt()
{
    C8ASSERT(current_token().token_type == ckx_token::type::tk_while);
    ckx_src_rng while_rng = current_token().rng;
    next_token();

    expect_n_eat(ckx_token::type::tk_lparen);
    ckx_ast_expr *condition = parse_expr();
    expect_n_eat(ckx_token::type::tk_rparen);
    ckx_ast_stmt *clause = parse_stmt();

    return new ckx_ast_while_stmt(while_rng, condition, clause);
}


ckx_ast_do_while_stmt*
ckx_parser_impl::parse_do_while_stmt()
{
    C8ASSERT(current_token().token_type == ckx_token::type::tk_do);
    ckx_src_rng do_rng = current_token().rng;
    next_token();

    ckx_ast_stmt *clause = parse_stmt();
    ckx_src_rng while_rng = expect_n_eat(ckx_token::type::tk_while).get();
    expect_n_eat(ckx_token::type::tk_lparen);
    ckx_ast_expr *condition = parse_expr();
    expect_n_eat(ckx_token::type::tk_rparen);
    expect_n_eat(ckx_token::type::tk_semicolon);

    return new ckx_ast_do_while_stmt(do_rng, while_rng, condition, clause);
}


ckx_ast_for_stmt*
ckx_parser_impl::parse_for_stmt()
{
    C8ASSERT(current_token().token_type == ckx_token::type::tk_for);
    ckx_src_rng for_rng = current_token().rng;
    next_token();

    expect_n_eat(ckx_token::type::tk_lparen);
    ckx_ast_expr *init = nullptr;
    ckx_ast_expr *cond = nullptr;
    ckx_ast_expr *incr = nullptr;
    if (current_token().token_type != ckx_token::type::tk_semicolon)
        init = parse_expr();
    expect_n_eat(ckx_token::type::tk_semicolon);
    if (current_token().token_type != ckx_token::type::tk_semicolon)
        cond = parse_expr();
    expect_n_eat(ckx_token::type::tk_semicolon);
    if (current_token().token_type != ckx_token::type::tk_rparen)
        incr = parse_expr();
    expect_n_eat(ckx_token::type::tk_rparen);

    ckx_ast_stmt *clause = parse_stmt();
    return new ckx_ast_for_stmt(for_rng, init, cond, incr, clause);
}


ckx_ast_break_stmt*
ckx_parser_impl::parse_break_stmt()
{
    C8ASSERT(current_token().token_type == ckx_token::type::tk_break);
    ckx_src_rng src_rng = current_token().rng;
    next_token();
    expect_n_eat(ckx_token::type::tk_semicolon);

    return new ckx_ast_break_stmt(src_rng);
}



ckx_ast_continue_stmt*
ckx_parser_impl::parse_continue_stmt()
{
    C8ASSERT(current_token().token_type == ckx_token::type::tk_continue);
    ckx_src_rng src_rng = current_token().rng;
    next_token();
    expect_n_eat(ckx_token::type::tk_semicolon);

    return new ckx_ast_continue_stmt(src_rng);
}


ckx_ast_return_stmt*
ckx_parser_impl::parse_return_stmt()
{
    C8ASSERT(current_token().token_type == ckx_token::type::tk_return);
    ckx_src_rng src_rng = current_token().rng;
    next_token();

    ckx_ast_expr *expr = nullptr;
    if (current_token().token_type != ckx_token::type::tk_semicolon)
        expr = parse_expr();
    expect_n_eat(ckx_token::type::tk_semicolon);

    return new ckx_ast_return_stmt(src_rng, expr);
}


ckx_ast_compound_stmt*
ckx_parser_impl::parse_compound_stmt()
{
    ckx_src_rng lbrace_rng = expect_n_eat(ckx_token::type::tk_lbrace).get();
    saber::vector<ckx_ast_stmt*> stmts;
    while (current_token().token_type != ckx_token::type::tk_rbrace)
    {
        stmts.push_back(parse_stmt());
    }
    ckx_src_rng rbrace_rng = expect_n_eat(ckx_token::type::tk_rbrace).get();

    return new ckx_ast_compound_stmt(lbrace_rng,rbrace_rng, saber::move(stmts));
}


ckx_ast_expr*
ckx_parser_impl::parse_expr()
{
    return parse_assign_expr();
}


ckx_ast_expr*
ckx_parser_impl::parse_init_expr()
{
    switch (current_token().token_type)
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
        return parse_array_expr();

    case ckx_token::type::tk_id:
        if (id_is_typename(current_token()))
            return parse_array_expr();
    default:
        return parse_expr();
    }
}


ckx_ast_expr*
ckx_parser_impl::parse_array_expr()
{
    ckx_prelexed_type array_type = parse_type();
    C8ASSERT(!array_type.get_prelexed_tokens().empty());

    ckx_ast_array_expr *ret = new ckx_ast_array_expr(saber::move(array_type));

    expect_n_eat(ckx_token::type::tk_lparen);
    if (current_token().token_type == ckx_token::type::tk_vi_literal)
    {
        ret->set_size(current_token().v.i64);
        next_token();
    }
    expect_n_eat(ckx_token::type::tk_rparen);

    if (current_token().token_type == ckx_token::type::tk_lbrace)
    {
        next_token();
        saber::vector<ckx_ast_expr*> init_list;
        while (1)
        {
            init_list.push_back(parse_init_expr());
            if (current_token().token_type == ckx_token::type::tk_rbrace)
                break;
            expect_n_eat(ckx_token::type::tk_comma);
        }
        expect_n_eat(ckx_token::type::tk_rbrace);
        ret->set_init_list(saber::move(init_list));
    }
    else if (current_token().token_type == ckx_token::type::tk_lparen)
    {
        next_token();
        ckx_ast_expr *start = parse_expr();
        ckx_ast_expr *finish = nullptr;
        if (current_token().token_type == ckx_token::type::tk_comma)
        {
            next_token();
            finish = parse_expr();
        }
        expect_n_eat(ckx_token::type::tk_rparen);
        ret->set_range(start, finish);
    }

    return ret;
}


ckx_ast_expr*
ckx_parser_impl::parse_cond_expr()
{
    ckx_ast_expr *binary_expr = parse_binary_expr(0);

    if (current_token().token_type == ckx_token::type::tk_ques)
    {
        ckx_src_rng ques_rng = current_token().rng;
        next_token();
        ckx_ast_expr *then_expr = parse_expr();
        ckx_src_rng colon_rng = expect_n_eat(ckx_token::type::tk_colon).get();
        ckx_ast_expr *else_expr = parse_expr();

        return new ckx_ast_cond_expr(
            ques_rng, colon_rng, binary_expr, then_expr, else_expr);
    }
    return binary_expr;
}


ckx_ast_expr*
ckx_parser_impl::parse_binary_expr(quint8 _prec)
{
    ckx_ast_expr *expr = parse_unary_expr();
    ckx_op op = ckx_op_helper::token2binary(current_token().token_type);
    quint8 new_prec;
    while (ckx_op_helper::is_binary(op)
           && (new_prec=ckx_op_helper::precedence(op)) >= _prec)
    {
        ckx_src_rng operator_rng = current_token().rng;
        next_token();
        ckx_ast_expr *rhs_expr = parse_binary_expr(new_prec+1);
        ckx_ast_expr *binary_expr =
            new ckx_ast_binary_expr(operator_rng, op, expr, rhs_expr);
        expr = binary_expr;
        op = ckx_op_helper::token2binary(current_token().token_type);
    }
    return expr;
}


ckx_ast_expr*
ckx_parser_impl::parse_assign_expr()
{
    ckx_ast_expr *expr = parse_cond_expr();

    ckx_op current_op = ckx_op_helper::token2binary(current_token().token_type);
    if (ckx_op_helper::is_assign(current_op))
    {
        ckx_src_rng operator_rng = current_token().rng;
        next_token();
        return new ckx_ast_binary_expr(
            operator_rng, current_op, expr, parse_assign_expr());
    }
    return expr;
}


ckx_ast_cast_expr*
ckx_parser_impl::parse_cast_expr()
{
    ckx_src_rng begin_source_rng = current_token().rng;

    ckx_ast_cast_expr::castop castop;
    switch (current_token().token_type)
    {
    case ckx_token::type::tk_static_cast:
        castop = ckx_ast_cast_expr::castop::cst_static; break;
    case ckx_token::type::tk_const_cast:
        castop = ckx_ast_cast_expr::castop::cst_const; break;
    case ckx_token::type::tk_reinterpret_cast:
        castop = ckx_ast_cast_expr::castop::cst_reinterpret; break;
    case ckx_token::type::tk_ckx_cast:
        castop = ckx_ast_cast_expr::castop::cst_ckx; break;
    default: C8ASSERT(false); // What the fuck!
    }
    ckx_src_rng kwd_rng = current_token().rng;
    next_token();
    expect_n_eat(ckx_token::type::tk_lt);
    ckx_prelexed_type desired_type = parse_type();
    expect_n_eat(ckx_token::type::tk_gt);
    expect_n_eat(ckx_token::type::tk_lparen);
    ckx_ast_expr *expr = parse_expr();
    expect_n_eat(ckx_token::type::tk_rparen);

    return new ckx_ast_cast_expr(
        kwd_rng, castop, saber::move(desired_type), expr);
}


ckx_ast_expr*
ckx_parser_impl::parse_unary_expr()
{
    ckx_token at_token = current_token();
    ckx_src_rng operator_rng = at_token.rng;

    switch (current_token().token_type)
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
            operator_rng,
            ckx_op_helper::token2unary(at_token.token_type),
            parse_unary_expr());

    default:
        return parse_postfix_expr();
    }
}


ckx_ast_expr*
ckx_parser_impl::parse_postfix_expr()
{
    ckx_src_rng begin_source_rng = current_token().rng;
    ckx_ast_expr *ret = parse_basic_expr();

    while (1)
    {
        switch (current_token().token_type)
        {
        case ckx_token::type::tk_lparen:
            {
                ckx_src_rng lparen_rng = current_token().rng;
                next_token();
                saber::vector<ckx_ast_expr*> args;
                while (current_token().token_type
                       != ckx_token::type::tk_rparen)
                {
                    args.push_back(parse_expr());

                    if (current_token().token_type == ckx_token::type::tk_comma)
                        next_token();
                }
                ckx_src_rng rparen_rng =
                    expect_n_eat(ckx_token::type::tk_rparen).get();
                ret = new ckx_ast_invoke_expr(lparen_rng, rparen_rng,
                                              ret, saber::move(args));
                break;
            }

        case ckx_token::type::tk_lbracket:
            {
                ckx_src_rng lbracket_rng = current_token().rng;
                next_token();
                ckx_ast_expr *subscript_expr = parse_expr();
                ckx_src_rng rbracket_rng =
                    expect_n_eat(ckx_token::type::tk_rbracket).get();
                ret = new ckx_ast_subscript_expr(lbracket_rng, rbracket_rng,
                                                 ret, subscript_expr);
                break;
            }

        case ckx_token::type::tk_dot:
            {
                next_token();
                ret = new ckx_ast_extract_expr(ret, current_token().str);
                next_token();
                break;
            }

        default:
            return ret;
        }
    }

    return nullptr;
}


ckx_ast_expr*
ckx_parser_impl::parse_basic_expr()
{
    switch (current_token().token_type)
    {
    case ckx_token::type::tk_vi_literal:
        {
            ckx_ast_expr* ret = new ckx_ast_vi_literal_expr(
                current_token().rng, current_token().v.i64);
            next_token();
            return ret;
        }

    case ckx_token::type::tk_vr_literal:
        {
            ckx_ast_expr* ret = new ckx_ast_vr_literal_expr(
                current_token().rng, current_token().v.r);
            next_token();
            return ret;
        }

    case ckx_token::type::tk_id:
        {
            ckx_src_rng id_rng = current_token().rng;
            saber_string_view name = current_token().str;
            if (id_is_typename(current_token()))
            {
                if (peek_next_token().token_type == ckx_token::type::tk_scope)
                {
                    next_token();
                    next_token();
                    expect(ckx_token::type::tk_id);
                    ckx_src_rng enumerator_rng = current_token().rng;
                    saber_string_view enumer = current_token().str;
                    ckx_ast_expr *ret =
                        new ckx_ast_enumerator_expr(
                            id_rng, enumerator_rng, name, enumer);
                    next_token();
                    return ret;
                }
            }
            next_token();
            return new ckx_ast_id_expr(id_rng, name);
        }

    case ckx_token::type::tk_lparen:
        {
            next_token();
            ckx_ast_expr *ret = parse_expr();
            expect_n_eat(ckx_token::type::tk_rparen);
            return ret;
        }

    case ckx_token::type::tk_sizeof:
        {
            ckx_src_rng begin_source_range = current_token().rng;

            next_token();
            expect_n_eat(ckx_token::type::tk_lparen);
            ckx_prelexed_type type = parse_type();
            expect_n_eat(ckx_token::type::tk_rparen);
            return new ckx_ast_sizeof_expr(
                ckx_src_rng::concat(begin_source_range,
                                         current_token().rng),
                saber::move(type));
        }

    default:
        return nullptr;
    }
}


ckx_prelexed_type
ckx_parser_impl::parse_type()
{
    saber::vector<ckx_token> prelexed_tokens;
    if (current_token().token_type == ckx_token::type::tk_id
        || (current_token().token_type >= ckx_token::type::tk_vi8
           && current_token().token_type <= ckx_token::type::tk_void))
    {
        prelexed_tokens.push_back(current_token());
        next_token();
    }
    else
    {
        return ckx_prelexed_type(saber::move(prelexed_tokens));
    }

    while (1)
    {
        switch ( current_token().token_type )
        {
        case ckx_token::type::tk_const:
        case ckx_token::type::tk_mul:
        case ckx_token::type::tk_arr:
            prelexed_tokens.push_back(current_token());
            next_token();
            break;

        default:
            return ckx_prelexed_type(saber::move(prelexed_tokens));
        }
    }
}


inline ckx_token&
ckx_parser_impl::current_token()
{
    return token_stream->operator[](0);
}


inline ckx_token&
ckx_parser_impl::peek_next_token()
{
    return token_stream->operator[](1);
}


inline void
ckx_parser_impl::next_token()
{
    return token_stream->operator++();
}

inline saber::optional<ckx_src_rng>
ckx_parser_impl::expect_n_eat(ckx_token::type _token_type)
{
    if (current_token().token_type == _token_type)
    {
        ckx_src_rng rng = current_token().rng;
        next_token();
        return saber::optional<ckx_src_rng>(rng);
    }
    syntax_error(current_token().rng,
                 saber_string_pool::create_view("Unexpected token"));
    return saber::optional<ckx_src_rng>();
}


inline saber::optional<ckx_src_rng>
ckx_parser_impl::expect(ckx_token::type _token_type)
{
    if (current_token().token_type == _token_type)
        return saber::optional<ckx_src_rng>(current_token().rng);

    syntax_error(current_token().rng,
                 saber_string_pool::create_view("Unexpected token"));
    return saber::optional<ckx_src_rng>();
}


bool
ckx_parser_impl::id_is_typename(ckx_token _token)
{
    C8ASSERT(_token.token_type == ckx_token::type::tk_id);
    return typename_table.query_typename(_token.str);
}


void
ckx_parser_impl::syntax_error(ckx_src_rng _rng,
                              saber_string_view _desc)
{
    error_list.emplace_back(_rng, _desc);
}


void
ckx_parser_impl::syntax_warn(ckx_src_rng _rng,
                             saber_string_view _desc)
{
    warn_list.emplace_back(_rng, _desc);
}


void
ckx_parser_impl::skip2_token(const ckx_token_set& _token_set)
{
    while (_token_set.find(current_token().token_type) == _token_set.end()
           && (current_token().token_type != ckx_token::type::tk_eoi))
        next_token();
}

} // namespace detail

} // namespace ckx
