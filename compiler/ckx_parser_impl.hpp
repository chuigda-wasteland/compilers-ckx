#ifndef CKX_PARSER_IMPL_HPP
#define CKX_PARSER_IMPL_HPP

#include "ckx_parser.hpp"
#include "ckx_type.hpp"
#include "ckx_token_set.hpp"
#include "ckx_error.hpp"
#include "ckx_env_table.hpp"

#include "ckx_ast_node.hpp"

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

    typename ckx_parser<CkxTokenStream>::parse_result
    parse_impl(saber_ptr<CkxTokenStream> _token_stream);

private:
    /// @brief General parsing functions
    /// for productions, see compilers-ckx/compiler/production.txt
    ckx_ast_stmt* parse_global_stmt();
    ckx_ast_stmt* parse_stmt();

    ckx_ast_decl_stmt*      parse_decl_stmt();
    ckx_ast_func_stmt*      parse_func_stmt();
    ckx_ast_enum_stmt*      parse_enum_stmt();
    ckx_ast_compound_stmt*  parse_ckx_block();

    ckx_ast_expr_stmt*      parse_expr_stmt();
    ckx_ast_if_stmt*        parse_if_stmt();
    ckx_ast_while_stmt*     parse_while_stmt();
    ckx_ast_do_while_stmt*  parse_do_while_stmt();
    ckx_ast_for_stmt*       parse_for_stmt();
    ckx_ast_break_stmt*     parse_break_stmt();
    ckx_ast_continue_stmt*  parse_continue_stmt();
    ckx_ast_return_stmt*    parse_return_stmt();
    ckx_ast_compound_stmt*  parse_compound_stmt();

    ckx_ast_expr*            parse_conditional_expr();
    ckx_ast_binary_expr*     parse_binary_expr();
    ckx_ast_cast_expr*       parse_cast_expr();
    ckx_ast_unary_expr*      parse_unary_expr();
    ckx_ast_subscript_expr*  parse_array_subscript();
    ckx_ast_invoke_expr*     parse_func_invoke();
    ckx_ast_id_expr*         parse_id();

    template <typename CkxAstRecordStmt, typename CkxRecordType>
    CkxAstRecordStmt* parse_record_stmt();

private:
    /// @brief individual functions for expressions
    /// @todo finish this part after we have a fully-designed expr-tree system
    ckx_ast_expr* parse_expr();

    /// @fn we need this function to ease the type resolving.
    saber_ptr<ckx_type> parse_type();

    /// @brief utility functions
    bool is_typename(saber_ptr<ckx_token> _token);

    /// @brief token-access functions
    /// simply encapsuled token-stream again.
    inline  saber_ptr<ckx_token>  current_token();
    inline  saber_ptr<ckx_token>  peek_next_token();
    inline  void                  next_token();
    inline  void                  expect_n_eat(ckx_token::type _token_type);
    inline  void                  expect(ckx_token::type _token_type);

    /// @brief interact with environment/symbol-table.
    inline  ckx_env*        env();
    inline  void            enter_scope(ckx_env* _new_scope_env);
    inline  void            leave_scope();

    /// @brief functions for reporting problem.
    void syntax_error(saber_string&& _reason, const qcoord& _pos);
    void syntax_warn(saber_string&& _reason, const qcoord& _pos);

    /// @brief and here are functions for recovering from a syntax error.
    /// @todo  implement these functions
    void skip2_token(const ckx_token_set& _token_set);

private:
    saber_ptr<CkxTokenStream> token_stream;
    ckx_env *current_env;

    saber::list<ckx_error*> *error_list = nullptr;
    saber::list<ckx_error*> *warn_list = nullptr;
};


} // namespace detail

} // namespace ckx

#endif // CKX_PARSER_IMPL_HPP
