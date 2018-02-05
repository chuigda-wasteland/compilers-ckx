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

ckx_context::context_type
ckx_context_manager::current_context_type_intern() const
{
    return context_chain.back()->type;
}

void ckx_context_manager::enter_func_context_intern(ckx_func_type *_func_type)
{
    context_chain.push_back(new ckx_func_context(_func_type));
}

void ckx_context_manager::exit_func_context_intern()
{
    delete context_chain.back();
    context_chain.pop_back();
}


ckx_context_manager::
func_context_raii::func_context_raii(ckx_context_manager &_context_manager,
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
