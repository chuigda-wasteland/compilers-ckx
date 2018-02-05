#include "frontend/sema/ckx_context_manager.hpp"

namespace ckx
{

ckx_context_manager::ckx_context_manager()
{
    context_chain.push_back(ckx_global_context());
}

ckx_context::context_type
ckx_context_manager::current_context_type_intern() const
{
    return (*context_chain.crbegin()).type;
}

void ckx_context_manager::enter_func_context_intern(ckx_func_type *_func_type)
{
    context_chain.push_back(ckx_func_context(_func_type));
}

void ckx_context_manager::exit_func_context_intern()
{
    context_chain.pop_back();
}


ckx_context_manager::
func_context_raii::func_context_raii(ckx_context_manager &&_context_manager,
                                     ckx_func_type *_func_type) :
    context_manager(_context_manager)
{
    context_manager.enter_func_context_intern(_func_type);
}

ckx_context_manager::
func_context_raii::~func_context_raii()
{
    C8ASSERT(context_manager.current_context_type_intern() \
             == ckx_context::context_type::cxt_func);
    context_manager.exit_func_context_intern();
}

} // namespace ckx
