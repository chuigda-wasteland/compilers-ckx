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

namespace ckx
{

namespace detail
{

template <typename CkxTokenStream>
class ckx_parser_impl
{
public:
    ckx_parser_impl() = default;
    ~ckx_parser_impl() = default;

    ckx_ast_translation_unit* parse_impl(
            saber_ptr<CkxTokenStream> _token_stream);

private:
    ckx_ast_stmt* parse_global_stmt();
    ckx_ast_stmt* parse_stmt();

    ckx_ast_decl_stmt* parse_decl_stmt();
    ckx_ast_func_stmt* parse_func_stmt();
    ckx_ast_struct_stmt* parse_struct_stmt();
    ckx_ast_variant_stmt* parse_variant_stmt();
    ckx_ast_enum_stmt* parse_enum_stmt();
    ckx_ast_compound_stmt* parse_ckx_block();

    ckx_ast_expr_stmt* parse_expr();
    ckx_ast_if_stmt* parse_if_stmt();
    ckx_ast_while_stmt* parse_while_stmt();
    ckx_ast_do_while_stmt *parse_do_while_stmt();
    ckx_ast_for_stmt* parse_for_stmt();
    ckx_ast_break_stmt* parse_break_stmt();
    ckx_ast_continue_stmt* parse_continue_stmt();
    ckx_ast_return_stmt* parse_return_stmt();
    ckx_ast_compound_stmt* parse_compound_stmt();

    ckx_ast_param_decl* parse_param();
    ckx_ast_init_decl *parse_init_decl();
    ckx_ast_expr* parse_expr();

    ckx_ast_struct* parse_struct();
    ckx_ast_variant* parse_variant();
    ckx_ast_enum* parse_enum();

    saber_ptr<ckx_type> parse_type();

    // For easing coding we replace the template TokenStream
    // with ckx_token_stream. Remember to replace back!
    Q_ON_HOLD(inline constexpr CkxTokenStream& stream();)
    inline constexpr ckx_token_stream& stream();

    inline void expect_n_eat(ckx_token::type _token_type);
    bool is_typename(const ckx_token& _token) const;

    saber_ptr<CkxTokenStream> token_stream;
    ckx_env_table *current_env;
};

} // namespace detail



template <typename CkxTokenStream>
ckx_parser::ckx_parser()
{
    p_impl = new detail::ckx_parser_impl<CkxTokenStream>;
}

template <typename CkxTokenStream>
ckx_parser::~ckx_parser()
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



} // namespace detail



} // namespace ckx
