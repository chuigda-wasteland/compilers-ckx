#ifndef CKX_PARSER_IMPL_HPP
#define CKX_PARSER_IMPL_HPP

#include "ckx_parser.hpp"
#include "ckx_type.hpp"
#include "ckx_token_set.hpp"
#include "ckx_error.hpp"
#include "ckx_env_table.hpp"
#include "ckx_ast_node.hpp"

#include "optional.hpp"

namespace ckx
{

namespace detail
{

class ckx_typename_table final
{
public:
    explicit ckx_typename_table() = default;
    ~ckx_typename_table() = default;

    void add_typename(saber_string_view _name)
    {
        typenames.emplace(_name);
    }

    bool query_typename(saber_string_view _name) const
    {
        if (typenames.find(_name) != typenames.end())
            return true;
        return false;
    }

    void cleanup()
    {
        typenames.clear();
    }

private:
    saber::unordered_set<saber_string_view, string_view_hash> typenames;
};


class ckx_parser_impl
{
public:
    explicit ckx_parser_impl() = default;
    ~ckx_parser_impl() = default;

    ckx_parser_impl(const ckx_parser_impl&) = delete;
    ckx_parser_impl(ckx_parser_impl&&) = delete;

    typename ckx_parser::parse_result
    parse_impl(ckx_token_stream* _token_stream);

protected:
    /// @brief General parsing functions
    /// for productions, see compilers-ckx/compiler/production.txt
    ckx_ast_stmt* parse_global_stmt();
    ckx_ast_stmt* parse_stmt();

    ckx_ast_decl_stmt*      parse_decl_stmt();
    ckx_ast_func_stmt*      parse_func_stmt();
    ckx_ast_record_stmt*    parse_record_stmt();
    ckx_ast_enum_stmt*      parse_enum_stmt();
    ckx_ast_alias_stmt*     parse_alias_stmt();

    ckx_ast_expr_stmt*      parse_expr_stmt();
    ckx_ast_if_stmt*        parse_if_stmt();
    ckx_ast_while_stmt*     parse_while_stmt();
    ckx_ast_do_while_stmt*  parse_do_while_stmt();
    ckx_ast_for_stmt*       parse_for_stmt();
    ckx_ast_break_stmt*     parse_break_stmt();
    ckx_ast_continue_stmt*  parse_continue_stmt();
    ckx_ast_return_stmt*    parse_return_stmt();
    ckx_ast_compound_stmt*  parse_compound_stmt();

    ckx_ast_expr*  parse_expr();
    ckx_ast_expr*  parse_init_expr();
    ckx_ast_expr*  parse_array_expr();

    ckx_ast_expr*  parse_cond_expr();
    ckx_ast_expr*  parse_binary_expr(quint8 _prec);
    ckx_ast_expr*  parse_assign_expr();
    ckx_ast_expr*  parse_unary_expr();
    ckx_ast_expr*  parse_postfix_expr();

    ckx_ast_cast_expr*       parse_cast_expr();
    ckx_ast_expr*            parse_basic_expr();
    ckx_ast_id_expr*         parse_id();

protected:

    /// @fn we need this function to ease the type resolving.
    ckx_prelexed_type parse_type();

    /// @brief utility functions
    bool id_is_typename(ckx_token _token);

    /// @brief token-access functions
    /// simply encapsuled token-stream again.
    inline  ckx_token  current_token();
    inline  ckx_token  peek_next_token();
    inline  void       next_token();

    inline saber::optional<ckx_source_range>
    expect_n_eat(ckx_token::type _token_type);

    inline saber::optional<ckx_source_range>
    expect(ckx_token::type _token_type);

    /// @brief functions for reporting problem.
    void syntax_error(ckx_source_range _rng, saber_string_view _desc);
    void syntax_warn(ckx_source_range _rng, saber_string_view _desc);

    /// @brief and here are functions for recovering from a syntax error.
    void skip2_token(const ckx_token_set& _token_set);

protected:
    ckx_token_stream *token_stream;

    saber::list<ckx_syntax_error> error_list;
    saber::list<ckx_syntax_error> warn_list;

    /// @note still we need a table for storing types occured in parsing
    ckx_typename_table typename_table;
};


} // namespace detail

} // namespace ckx

#endif // CKX_PARSER_IMPL_HPP
