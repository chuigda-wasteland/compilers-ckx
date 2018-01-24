#include "ckx_ast_node.hpp"

#include "ckx_sema.hpp"
#include "ckx_sema_result.hpp"

namespace ckx
{

/// @todo this is just for suppressing warnings.
using bwsb = saber::optional<ckx_expr_result>;

void ckx_ast_compound_stmt::accept(ckx_sema_engine&) {}
void ckx_ast_if_stmt::accept(ckx_sema_engine&) {}
void ckx_ast_while_stmt::accept(ckx_sema_engine&) {}
void ckx_ast_do_while_stmt::accept(ckx_sema_engine&) {}
void ckx_ast_for_stmt::accept(ckx_sema_engine&) {}
void ckx_ast_break_stmt::accept(ckx_sema_engine&) {}
void ckx_ast_continue_stmt::accept(ckx_sema_engine&) {}
void ckx_ast_return_stmt::accept(ckx_sema_engine&) {}
void ckx_ast_decl_stmt::accept(ckx_sema_engine&) {}
void ckx_ast_expr_stmt::accept(ckx_sema_engine&) {}
void ckx_ast_func_stmt::accept(ckx_sema_engine&) {}
void ckx_ast_record_stmt::accept(ckx_sema_engine&) {}
void ckx_ast_alias_stmt::accept(ckx_sema_engine&) {}
void ckx_ast_enum_stmt::accept(ckx_sema_engine&) {}
saber::optional<ckx_expr_result>
ckx_ast_binary_expr::accept(ckx_sema_engine&) { return bwsb(); }
saber::optional<ckx_expr_result>
ckx_ast_unary_expr::accept(ckx_sema_engine&) { return bwsb(); }
saber::optional<ckx_expr_result>
ckx_ast_subscript_expr::accept(ckx_sema_engine&) { return bwsb(); }
saber::optional<ckx_expr_result>
ckx_ast_invoke_expr::accept(ckx_sema_engine&) { return bwsb(); }
saber::optional<ckx_expr_result>
ckx_ast_extract_expr::accept(ckx_sema_engine&) { return bwsb(); }
saber::optional<ckx_expr_result>
ckx_ast_enumerator_expr::accept(ckx_sema_engine&) { return bwsb(); }
saber::optional<ckx_expr_result>
ckx_ast_cond_expr::accept(ckx_sema_engine&) { return bwsb(); }
saber::optional<ckx_expr_result>
ckx_ast_id_expr::accept(ckx_sema_engine&) { return bwsb(); }
saber::optional<ckx_expr_result>
ckx_ast_cast_expr::accept(ckx_sema_engine&) { return bwsb(); }
saber::optional<ckx_expr_result>
ckx_ast_sizeof_expr::accept(ckx_sema_engine&) { return bwsb(); }
saber::optional<ckx_expr_result>
ckx_ast_vi_literal_expr::accept(ckx_sema_engine&) { return bwsb(); }
saber::optional<ckx_expr_result>
ckx_ast_vr_literal_expr::accept(ckx_sema_engine&) { return bwsb(); }
saber::optional<ckx_expr_result>
ckx_ast_array_expr::accept(ckx_sema_engine&) { return bwsb(); }

} // namespace ckx
