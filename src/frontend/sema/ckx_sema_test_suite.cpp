#include "frontend/sema/ckx_sema.hpp"

namespace ckx
{

void ckx_sema_engine::test_print(we::we_file_writer &writer)
{
    builder.pretty_print(writer);
}

void ckx_sema_engine::test_enter_scope()
{
    enter_scope();
}

void ckx_sema_engine::test_leave_scope()
{
    leave_scope();
}

} // namespace ckx
