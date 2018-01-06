#include "ckx_ast_node.hpp"

#include "ckx_sema.hpp"
#include "ckx_sema_result.hpp"

namespace ckx
{


void ckx_ast_compound_stmt::accept(ckx_sema_engine &sema)
{
    if (!sema.is_in_func())
    {
        sema.error();
        return;
    }

    for (auto stmt : stmts) stmt->accept(sema);
}

void ckx_ast_return_stmt::accept(ckx_sema_engine &sema)
{
}



} // namespace ckx
