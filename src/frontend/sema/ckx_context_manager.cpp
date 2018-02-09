#include "frontend/sema/ckx_context_manager.hpp"

namespace ckx
{

ckx_context_manager::ckx_context_manager()
{
    context_chain.push_back(new ckx_global_context());
}

ckx_func_context* ckx_context_manager::lookup_func_context()
{
    for (auto it = context_chain.rbegin(); it != context_chain.rend(); ++it)
    {
        if ((*it)->type == ckx_context::context_type::cxt_func)
        {
            return static_cast<ckx_func_context*>(*it);
        }
    }

    return nullptr;
}

void ckx_context_manager::enter_func_context(ckx_func_type *_func_type)
{
    context_chain.push_back(new ckx_func_context(_func_type));
}

void ckx_context_manager::exit_func_context()
{
    C8ASSERT(context_chain.back()->type == ckx_context::context_type::cxt_func);
    delete context_chain.back();
    context_chain.pop_back();
}

ckx_context::context_type
ckx_context_manager::current_context_type_intern() const
{
    return context_chain.back()->type;
}


} // namespace ckx
