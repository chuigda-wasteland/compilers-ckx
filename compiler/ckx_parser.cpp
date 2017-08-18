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
    ckx_ast_stmt* parse_stmt();

    ckx_ast_decl* parse_decl();
    ckx_ast_function* parse_function();

    ckx_ast_compound_stmt* parse_compound_stmt();
    ckx_ast_if_stmt* parse_if_stmt();
    ckx_ast_while_stmt* parse_while_stmt();
    ckx_ast_for_stmt* parse_for_stmt();
    ckx_ast_return_stmt* parse_return_stmt();

    ckx_ast_expr_stmt* parse_expr_stmt();
    ckx_ast_expr* parse_expr();

    ckx_ast_struct_decl* parse_struct_decl();
    ckx_ast_variant_decl* parse_variant_decl();
    ckx_ast_enum_decl* parse_enum_decl();

    /// @todo
    // For easing coding we replace the template TokenStream
    // with ckx_token_stream. Remember to replace back!
    inline constexpr ckx_token_stream& stream();
    // TokenStream& stream();

    inline void expect_n_eat(ckx_token::type _token_type);
    bool is_typename(const ckx_token& _token) const;

    saber_ptr<TokenStream> token_stream;
};

template <typename TokenStream>
ckx_ast_stmt*
ckx_parser_impl<TokenStream>::parse_stmt()
{
    switch (stream()[0]->token_type)
    {
    case ckx_token::type::token_identifier:
        // An identifier may be
        //     -- (1) a typename
        //     -- (2) a function name
        //     -- (3) an enumeration constant or a variable

        // If an identifier is a typename, then definitely this stmt shall
        // be a declaration. Otherwise, it should part of expression-stmt

        if ( is_typename(*stream()[0]) ) goto id_as_typename;
        return parse_expr_stmt();

    case ckx_token::type::token_int8:
    case ckx_token::type::token_int16:
    case ckx_token::type::token_int32:
    case ckx_token::type::token_int64:
    case ckx_token::type::token_uint8:
    case ckx_token::type::token_uint16:
    case ckx_token::type::token_uint32:
    case ckx_token::type::token_uint64:
    case ckx_token::type::token_char:
    case ckx_token::type::token_real32:
    case ckx_token::type::token_int64:
        id_as_typename:
        return parse_decl();

    case ckx_token::type::token_function:
        return parse_function();

    case ckx_token::type::token_if:
        return parse_if_stmt();

    case ckx_token::type::token_while:
        return parse_while_stmt();

    case ckx_token::type::token_for:
        return parse_for_stmt();

    case ckx_token::type::token_return:
        return parse_return_stmt();

    case ckx_token::type::token_break:
        return new ckx_ast_break_stmt();

    case ckx_token::type::token_continue:
        return new ckx_ast_continue_stmt();

    case ckx_token::type::token_struct: break; // On hold


    case ckx_token::type::token_variant: break; // On hold


    case ckx_token::type::token_lbrace:
        return parse_compound_stmt();

    }
}

template <typename TokenStream>
ckx_ast_compound_stmt*
ckx_parser_impl<TokenStream>::parse_compound_stmt()
{
    ckx_ast_compound_stmt* ret = new ckx_ast_compound_stmt();

    ++stream();
    while (stream()[0].get()->token_type
           != ckx_token::type::token_rbrace)
    {
        ret->add_stmt(parse_stmt());
    }

    ++stream();

    return ret;
}

template <typename TokenStream>
ckx_ast_if_stmt*
ckx_parser_impl<TokenStream>::parse_if_stmt()
{
    ++stream();

    // Recovering form error is on hold.
    expect_n_eat(ckx_token::type::token_lparth);
    ckx_ast_expr* condition = parse_expr();
    expect_n_eat(ckx_token::type::token_rparth);

    ckx_ast_stmt *then_clause = parse_stmt();
    ckx_ast_stmt *else_clause = nullptr;
    if (stream()[0]->token_type != ckx_token::type::token_else)
    {
        else_clause = parse_stmt();
    }

    return new ckx_ast_if_stmt(condition, then_clause, else_clause);
}

template <typename TokenStream>
ckx_ast_while_stmt*
ckx_parser_impl<TokenStream>::parse_while_stmt()
{
    ++stream();

    expect_n_eat(ckx_token::type::token_lparth);
    ckx_ast_expr* condition = parse_expr();
    expect_n_eat(ckx_token::type::token_rparth);
    ckx_ast_stmt *while_clause = parse_stmt();

    return new ckx_ast_while_stmt(condition, while_clause);
}

template <typename TokenStream>
ckx_ast_for_stmt*
ckx_parser_impl<TokenStream>::parse_for_stmt()
{
    // On hold.
}

template <typename TokenStream>
ckx_ast_return_stmt*
ckx_parser_impl<TokenStream>::parse_return_stmt()
{
    return new ckx_ast_return_stmt(parse_expr());
}

template <typename TokenStream>
inline void
ckx_parser_impl<TokenStream>::expect_n_eat(ckx_token::type _token_type)
{
    if (stream()[0]->token_type == _token_type)
        ++stream();
    else
        /*On hold*/ ;
}

template <typename TokenStream>
inline constexpr ckx_token_stream&
// inline constexpr TokenStream&
ckx_parser_impl<TokenStream>::stream()
{
    return *token_stream;
}

} // namespace detail

} // namespace ckx
