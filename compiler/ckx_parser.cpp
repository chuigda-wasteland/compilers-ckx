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

namespace ckx
{

namespace detail
{

template <typename TokenStream>
class ckx_parser_impl
{
public:
    ckx_parser_impl();
    ~ckx_parser_impl();

    ckx_ast_translation_unit* parse_impl(saber_ptr<TokenStream> _token_stream);

private:
    ckx_ast_stmt* parse_statement();

    ckx_ast_decl* parse_decl();
    ckx_ast_function* parse_function();

    ckx_ast_compound_stmt* parse_compound_statement();
    ckx_ast_if_stmt* parse_if_statement();
    ckx_ast_while_stmt* parse_while_statement();
    ckx_ast_for_stmt* parse_for_statement();

    ckx_ast_expr* parse_expr();

    ckx_ast_struct_decl* parse_struct_decl();
    ckx_ast_variant_decl* parse_variant_decl();
    ckx_ast_enum_decl* parse_enum_decl();

    // For coding we replace the template TokenStream with ckx_token_stream
    ckx_token_stream& get_stream();

    inline void expect_n_eat(ckx_token::type _type);

    saber_ptr<TokenStream> token_stream;
};

template <typename TokenStream>
ckx_ast_compound_stmt* ckx_parser_impl<TokenStream>::parse_compound_statement()
{
    ckx_ast_compound_stmt* ret = new ckx_ast_compound_stmt();

    ++get_stream();
    while (get_stream()[0].get()->token_type
           != ckx_token::type::token_rbrace)
    {
        ret->add_stmt(parse_statement());
    }

    ++get_stream();

    return ret;
}

template <typename TokenStream>
ckx_ast_if_stmt* ckx_parser_impl<TokenStream>::parse_if_statement()
{
    ++get_stream();

    // Recovering form error is on hold.
    expect_n_eat(ckx_token::type::token_lparth);
    ckx_ast_expr* condition = parse_expr();
    expect_n_eat(ckx_token::type::token_rparth);

    ckx_ast_stmt *then_clause = parse_statement();
    ckx_ast_stmt *else_clause = nullptr;
    if (get_stream()[0].get()->token_type
            != ckx_token::type::token_else)
    {
        else_clause = parse_statement();
    }

    return new ckx_ast_if_stmt(condition, then_clause, else_clause);
}

template <typename TokenStream>
ckx_ast_while_stmt* ckx_parser_impl<TokenStream>::parse_while_statement()
{
    ++get_stream();

    expect_n_eat(ckx_token::type::token_lparth);
    ckx_ast_expr* condition = parse_expr();
    expect_n_eat(ckx_token::type::token_rparth);

    ckx_ast_stmt *while_clause = parse_statement();

    return new ckx_ast_while_stmt(condition, while_clause);
}

template <typename TokenStream>
ckx_ast_for_stmt* ckx_parser_impl<TokenStream>::parse_for_statement()
{
    // On hold.
}

template <typename TokenStream>
inline void ckx_parser_impl<TokenStream>::expect_n_eat(ckx_token::type _type)
{
    if (get_stream()[0].get()->token_type == _type)
        ++get_stream();
    else
        /*On hold*/ ;
}

} // namespace detail

} // namespace ckx
