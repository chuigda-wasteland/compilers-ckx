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

#include "ckx_parser.hpp"

#include "ckx_ast_node.hpp"
#include "ckx_type.hpp"
#include "ckx_token_set.hpp"
#include "ckx_error.hpp"

namespace ckx
{

namespace detail
{

template <typename CkxTokenStream>
class ckx_parser_impl
{
public:
    explicit ckx_parser_impl() = default;
    ~ckx_parser_impl() = default;

    ckx_ast_translation_unit* parse_impl(
            saber_ptr<CkxTokenStream> _token_stream);

private:
    /// @brief General parsing functions
    /// for productions, see compilers-ckx/compiler/production.txt
    ckx_ast_stmt* parse_global_stmt();
    ckx_ast_stmt* parse_stmt();

    ckx_ast_decl_stmt* parse_decl_stmt();
    ckx_ast_func_stmt* parse_func_stmt();
    ckx_ast_struct_stmt* parse_struct_stmt();
    ckx_ast_variant_stmt* parse_variant_stmt();
    ckx_ast_enum_stmt* parse_enum_stmt();
    ckx_ast_compound_stmt* parse_ckx_block();

    ckx_ast_expr_stmt* parse_expr_stmt();
    ckx_ast_if_stmt* parse_if_stmt();
    ckx_ast_while_stmt* parse_while_stmt();
    ckx_ast_do_while_stmt *parse_do_while_stmt();
    ckx_ast_for_stmt* parse_for_stmt();
    ckx_ast_break_stmt* parse_break_stmt();
    ckx_ast_continue_stmt* parse_continue_stmt();
    ckx_ast_return_stmt* parse_return_stmt();
    ckx_ast_compound_stmt* parse_compound_stmt();

private:
    /// @brief individual functions for expressions
    /// @todo finish this part after we have a fully-designed expr-tree system
    ckx_ast_expr* parse_expr();

    /// @fn we need this function to ease the type resolving.
    saber_ptr<ckx_type> parse_type();

    /// @fn this function (these two functions) resolves struct/variant fields
    template <typename RecordType>
    void parse_record_type_fields(RecordType& _type);

    /// @brief utility functions
    bool is_typename(saber_ptr<ckx_token> _token) const;

    /// @brief token-access functions
    /// simply encapsuled token-stream again.
    inline saber_ptr<ckx_token> current_token();
    inline saber_ptr<ckx_token> peek_next_token();
    inline void move2_next_token();
    inline void expect_n_eat(ckx_token::type _token_type);
    inline void expect(ckx_token::type _token_type);

    /// @brief interact with environment/symbol-table.
    inline ckx_env_table* env();
    inline void enter_scope(ckx_env_table* _new_scope_env);
    inline void leave_scope();

    /// @brief functions for reporting problem.
    void syntax_error(saber::string&& _reason, const qcoord& _pos);
    void syntax_warn(saber::string&& _reason, const qcoord& _pos);

    /// @brief and here are functions for recovering from a syntax error.
    /// @todo  implement these functions
    void skip2_token(const ckx_token_set& _token_set);

private:
    saber_ptr<CkxTokenStream> token_stream;
    ckx_env_table *current_env;
};

} // namespace detail



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
ckx_ast_translation_unit*
ckx_parser<CkxTokenStream>::parse(saber_ptr<CkxTokenStream> _token_stream)
{
    return p_impl->parse_impl(_token_stream);
}

namespace detail
{

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

    case ckx_token::type::tk_function:
        return parse_func_stmt();

    case ckx_token::type::tk_struct:
        return parse_struct_stmt();

    case ckx_token::type::tk_variant:
        return parse_variant_stmt();

    case ckx_token::type::tk_enum:
        return parse_enum_stmt();

    case ckx_token::type::tk_ckx:
        return parse_ckx_block();

    /// @todo Module manage system still designing.
    /// We will have C-like preprocessors before creating a powerful
    /// module manage system.
    Q_ON_HOLD(case ckx_token::type::tk_import:)
    Q_ON_HOLD(case ckx_token::type::tk_export:)

    case ckx_token::type::tk_id:
        if ( is_typename(current_token()) )
            return parse_decl_stmt();

    default:
        syntax_error(
            "Expected : typename, 'function', 'struct', 'variant' or 'enum'"
            " as the commemce of global declaration.",
            current_token()->position);
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

    /// @todo We met some trouble with enum type.
    case ckx_token::type::tk_id:
        if ( env()->lookup_type(*(current_token()->v.p_str)) != nullptr )
            return parse_decl_stmt();
        else
            return parse_expr_stmt();

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
        move2_next_token();
        break;

    default:
        Q_ON_HOLD( issue_error(current_token(), "...") )
        ;
    }
}

template <typename CkxTokenStream>
ckx_ast_decl_stmt*
ckx_parser_impl<CkxTokenStream>::parse_decl_stmt()
{
    ckx_ast_decl_stmt* ret = new ckx_ast_decl_stmt(current_token());
    saber_ptr<ckx_type> type = parse_type();

    while (1)
    {
        saber_ptr<ckx_token> token = current_token();
        ckx_ast_expr *init = nullptr;
        if ( peek_next_token()->token_type == ckx_token::type::tk_assign )
        {
            move2_next_token();
            move2_next_token();
            init = parse_expr();
        }

        auto status = env()->add_new_var(*(token->v.p_str), type);
        ret->add_decl(new ckx_ast_init_decl(token, status.second, init));

        if ( peek_next_token()->token_type == ckx_token::type::tk_semicolon )
        {
            move2_next_token();
            break;
        }
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
    move2_next_token();

    saber::string func_name = *(current_token()->v.p_str);

    expect_n_eat(ckx_token::type::tk_lparth);
    ckx_env_table *param_env = new ckx_env_table(env());
    saber::vector<saber_ptr<ckx_type>> param_type_list;
    enter_scope(param_env);
    while (1)
    {
        saber_ptr<ckx_type> arg_type = parse_type();
        saber::string arg_name = *(current_token()->v.p_str);
        env()->add_new_var(arg_name, arg_type);
        param_type_list.push_back(arg_type);

        if (current_token()->token_type == ckx_token::type::tk_comma)
            continue;
        if (current_token()->token_type == ckx_token::type::tk_rparth)
            break;
    }
    leave_scope();
    expect_n_eat(ckx_token::type::tk_rparth);
    expect_n_eat(ckx_token::type::tk_colon);
    saber_ptr<ckx_type> ret_type = parse_type();

    ckx_func_type *func_type =
            new ckx_func_type(ret_type, saber::move(param_type_list));
    auto add_result = env()->add_new_func(func_name, func_type);

    if (current_token()->token_type == ckx_token::type::tk_lbrace)
    {
        enter_scope(param_env);
        add_result.second.the_function->define(parse_compound_stmt());
        leave_scope();
    }
    else if (current_token()->token_type == ckx_token::type::tk_semicolon)
    {
        move2_next_token();
    }

    return new ckx_ast_func_stmt(at_token, add_result.second, param_env);
}

template <typename CkxTokenStream>
ckx_ast_struct_stmt*
ckx_parser_impl<CkxTokenStream>::parse_struct_stmt()
{
    assert(current_token()->token_type == ckx_token::type::tk_struct);

    saber_ptr<ckx_token> at_token = current_token();
    move2_next_token();

    saber::string struct_typename = *(current_token()->v.p_str);
    move2_next_token();
    expect_n_eat(ckx_token::type::tk_lbrace);

    ckx_struct_type *type = new ckx_struct_type;
    saber_ptr<ckx_type> saber_type(type);
    auto add_result = env()->add_new_type(struct_typename, saber_type);

    parse_record_type_fields(*type);

    expect_n_eat(ckx_token::type::tk_rbrace);
    return new ckx_ast_struct_stmt(at_token, add_result.second);
}

template <typename CkxTokenStream>
ckx_ast_variant_stmt*
ckx_parser_impl<CkxTokenStream>::parse_variant_stmt()
{
    assert(current_token()->token_type == ckx_token::type::tk_variant);

    saber_ptr<ckx_token> at_token = current_token();
    move2_next_token();

    saber::string variant_typename = *(current_token()->v.p_str);
    move2_next_token();
    expect_n_eat(ckx_token::type::tk_lbrace);

    ckx_variant_type *type = new ckx_variant_type;
    auto add_result =
            env()->add_new_type(variant_typename, saber_ptr<ckx_type>(type));

    parse_record_type_fields(*type);

    expect_n_eat(ckx_token::type::tk_rbrace);
    return new ckx_ast_variant_stmt(at_token, add_result.second);
}

template <typename CkxTokenStream>
ckx_ast_enum_stmt*
ckx_parser_impl<CkxTokenStream>::parse_enum_stmt()
{
    assert(current_token()->token_type == ckx_token::type::tk_enum);
    saber_ptr<ckx_token> at_token = current_token();
    move2_next_token();

    saber::string enum_typename = *(current_token()->v.p_str);
    move2_next_token();
    expect_n_eat(ckx_token::type::tk_lbrace);

    ckx_enum_type *type = new ckx_enum_type;
    auto add_result =
            env()->add_new_type(enum_typename, saber_ptr<ckx_type>(type));

    while (current_token()->token_type != ckx_token::type::tk_rbrace)
    {
        saber::string enumerator_name = *(current_token()->v.p_str);
        expect_n_eat(ckx_token::type::tk_assign);
        qint64 enumerator_value = current_token()->v.i64;

        type->add_enumerator(saber::move(enumerator_name), enumerator_value);
    }

    expect_n_eat(ckx_token::type::tk_rbrace);
    return new ckx_ast_enum_stmt(at_token, add_result.second);
}

template <typename CkxTokenStream>
ckx_ast_compound_stmt*
ckx_parser_impl<CkxTokenStream>::parse_ckx_block()
{
    assert(current_token()->token_type == ckx_token::type::tk_ckx);
    saber_ptr<ckx_token> at_token = current_token();
    move2_next_token();

    expect(ckx_token::type::tk_lbrace);
    return parse_compound_stmt();
}

template <typename CkxTokenStream>
ckx_ast_expr_stmt*
ckx_parser_impl<CkxTokenStream>::parse_expr_stmt()
{
    /// @todo add code here after designing full expression ast
    return nullptr;
}

template <typename CkxTokenStream>
ckx_ast_if_stmt*
ckx_parser_impl<CkxTokenStream>::parse_if_stmt()
{
    assert(current_token()->token_type == ckx_token::type::tk_if);
    saber_ptr<ckx_token> at_token = current_token();
    move2_next_token();

    expect_n_eat(ckx_token::type::tk_lparth);
    ckx_ast_expr *condition = parse_expr();
    expect_n_eat(ckx_token::type::tk_rparth);

    ckx_ast_stmt *then_clause = parse_stmt();
    ckx_ast_stmt *else_clause = nullptr;

    if (current_token()->token_type == ckx_token::type::tk_else)
    {
        move2_next_token();
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
    move2_next_token();

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
    move2_next_token();

    ckx_ast_stmt *clause = parse_stmt();
    expect_n_eat(ckx_token::type::tk_while);
    expect_n_eat(ckx_token::type::tk_lparth);
    ckx_ast_expr *condition = parse_expr();
    expect_n_eat(ckx_token::type::tk_rparth);

    return new ckx_ast_do_while_stmt(at_token, condition, clause);
}

template <typename CkxTokenStream>
ckx_ast_break_stmt*
ckx_parser_impl<CkxTokenStream>::parse_break_stmt()
{
    assert(current_token()->token_type == ckx_token::type::tk_break);
    saber_ptr<ckx_token> at_token = current_token();
    move2_next_token();
    expect_n_eat(ckx_token::type::tk_semicolon);

    return new ckx_ast_break_stmt(at_token);
}

template <typename CkxTokenStream>
ckx_ast_continue_stmt*
ckx_parser_impl<CkxTokenStream>::parse_continue_stmt()
{
    assert(current_token()->token_type == ckx_token::type::tk_continue);
    saber_ptr<ckx_token> at_token = current_token();
    move2_next_token();
    expect_n_eat(ckx_token::type::tk_semicolon);

    return new ckx_ast_continue_stmt(at_token);
}

template <typename CkxTokenStream>
ckx_ast_return_stmt*
ckx_parser_impl<ckx_token_stream>::parse_return_stmt()
{
    assert(current_token()->token_type == ckx_token::type::tk_return);
    saber_ptr<ckx_token> at_token = current_token();
    move2_next_token();

    ckx_ast_expr *expr = nullptr;
    if (current_token() != ckx_token::type::tk_semicolon)
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

    ckx_env_table *new_env = new ckx_env_table(env());

    ckx_ast_compound_stmt *ret = new ckx_ast_compound_stmt(at_token, new_env);

    enter_scope(new_env);
    while (current_token() != ckx_token::type::tk_rbrace)
    {
        ret->add_new_stmt(parse_stmt());
    }

    expect_n_eat(ckx_token::type::tk_rbrace);
    leave_scope();
}


template <typename CkxTokenStream>
saber_ptr<ckx_type>
ckx_parser_impl<CkxTokenStream>::parse_type()
{
    saber_ptr<ckx_type> type;
    if (current_token()->token_type == ckx_token::type::tk_id)
    {
        type = env()->lookup_var( *(current_token()->v.p_str) );
    }
    else
    {
        type = ckx_type_helper::get_type( current_token()->token_type );
    }
    move2_next_token();

    while (1)
    {
        switch ( current_token() )
        {
        case ckx_token::type::tk_const:
            type = ckx_type_helper::qual_const(type); break;

        case ckx_token::type::tk_mul:
            type = ckx_type_helper::pointer_to(type); break;

        default:
            return type;
        }
    }
}

template <typename CkxTokenStream>
template <typename RecordType>
void
ckx_parser_impl<CkxTokenStream>::parse_record_type_fields(RecordType& _type)
{
    static_assert(
        saber::traits::type_equivalent<ckx_struct_type, RecordType>::value
        || saber::traits::type_equivalent<ckx_variant_type, RecordType>::value,
        "I don't know what the hell, just go and fuck yourself.");

    while (1)
    {
        saber_ptr<ckx_type> type = parse_type();

        while (1)
        {
            saber::string dclr_name = *(current_token()->v.p_str);
            _type.add_field(saber::move(dclr_name), type);

            if (current_token()->token_type == ckx_token::type::tk_comma)
                { move2_next_token(); continue; }

            if (current_token()->token_type == ckx_token::type::tk_semicolon)
                { move2_next_token(); break; }
        }

        if ( current_token()->token_type == ckx_token::type::tk_rbrace )
            return;
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
ckx_parser_impl<CkxTokenStream>::move2_next_token()
{
    return token_stream.get()->operator++();
}

template <typename CkxTokenStream>
inline void
ckx_parser_impl<CkxTokenStream>::expect_n_eat(ckx_token::type _token_type)
{
    if (current_token()->token_type == _token_type)
        move2_next_token();

    syntax_error("Unexpected token", current_token()->position);
}

/*
template <typename CkxTokenStream>
inline bool
ckx_parser_impl<CkxTokenStream>::expect(ckx_token::type _token_type)
{
    if (current_token()->token_type == _token_type)
        return true;

    syntax_error("Unexpected token", current_token()->position);
    return false;
}
*/

template <typename CkxTokenStream>
inline ckx_env_table*
ckx_parser_impl<CkxTokenStream>::env()
{
    return current_env;
}

template <typename CkxTokenStream>
inline void
ckx_parser_impl<CkxTokenStream>::enter_scope(ckx_env_table *_new_scope_env)
{
    current_env = _new_scope_env;
}

template <typename CkxTokenStream>
inline void
ckx_parser_impl<CkxTokenStream>::leave_scope()
{
    current_env = current_env->get_parent();
}

template <typename CkxTokenStream>
bool
ckx_parser_impl<CkxTokenStream>::is_typename(saber_ptr<ckx_token> _token) const
{
    return env()->lookup_type(_token->v.p_str);
}



template <typename CkxTokenStream>
void
ckx_parser_impl<CkxTokenStream>::syntax_error(saber::string &&_reason,
                                              const qcoord &_pos)
{
    /// @todo finalize this error processing system.
}

template <typename CkxTokenStream>
void
ckx_parser_impl<CkxTokenStream>::syntax_warn(saber::string&& _reason,
                                             const qcoord& _pos)
{
    /// @todo finalize this error processing system.
}


template <typename CkxTokenStream>
void
ckx_parser_impl<CkxTokenStream>::skip2_token(const ckx_token_set& _token_set)
{
    while ( (!_token_set.find(current_token()->token_type))
            && (current_token()->token_type != ckx_token::type::tk_eoi) )
        move2_next_token();
}


} // namespace detail

} // namespace ckx
