#ifndef CKX_SEMA_HPP
#define CKX_SEMA_HPP

#include "frontend/parse/ckx_prelexed_type.hpp"
#include "frontend/AST/ckx_ast_node.hpp"
#include "frontend/sema/ckx_type.hpp"
#include "frontend/sema/ckx_env_table.hpp"
#include "frontend/sema/ckx_sema_result.hpp"
#include "llvm/llvm_ir_builder.hpp"

namespace ckx
{

class ckx_sema_engine
{
    friend int main();
public:
    ckx_sema_engine() = default;
    ~ckx_sema_engine() = default;

    ckx_sema_engine(const ckx_sema_engine&) = delete;
    ckx_sema_engine(ckx_sema_engine&&) = delete;

    bool is_in_func() const { return in_func; }
    void error() {}

    ckx_expr_result decay_to_rvalue(ckx_expr_result _expr);

    void visit_decl_node(ckx_ast_decl_stmt*);

    saber::optional<ckx_expr_result> try_implicit_cast(ckx_expr_result _expr);
    saber::optional<ckx_expr_result> try_static_cast(ckx_expr_result _expr);
    saber::optional<ckx_expr_result> try_const_cast(ckx_expr_result _expr);
    saber::optional<ckx_expr_result> try_reinter_cast(ckx_expr_result _expr);
    saber::optional<ckx_expr_result> try_ckx_cast(ckx_expr_result _expr);

    /// @note for test only.
    void test_print(we::we_file_writer& writer);

private:
    ckx_env root_env;
    ckx_env *current_env = &root_env;
    faker::llvm_ir_builder builder;

    saber::optional<ckx_type_result>
    re_lex_type(const ckx_prelexed_type &_prelexed_type);
    static saber_string map_basic_type(ckx_type::category _basic_type);

    quint64 var_name_mangle_count() { return ++vname_mangle_count; }

    bool in_func = false;
    quint64 vname_mangle_count = 0;
};

} // namespace ckx

#endif // CKX_SEMA_HPP
