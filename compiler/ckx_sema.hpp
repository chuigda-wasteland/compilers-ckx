#ifndef CKX_SEMA_HPP
#define CKX_SEMA_HPP

#include "ckx_type.hpp"
#include "ckx_prelexed_type.hpp"
#include "ckx_ast_node.hpp"
#include "ckx_env_table.hpp"
#include "../llvm_ir_builder/llvm_ir_builder.hpp"

#include "ckx_sema_result.hpp"

namespace ckx
{

class ckx_sema_engine
{
public:
    bool is_in_func() const { return in_func; }
    void error() {}

    ckx_expr_result decay_to_rvalue(ckx_expr_result _expr);
    saber::optional<ckx_expr_result> try_implicit_cast(ckx_expr_result expr);

private:
    ckx_env root_env;
    ckx_env *current_env;
    faker::llvm_ir_builder *builder;

    bool in_func;
};

} // namespace ckx

#endif // CKX_SEMA_HPP
