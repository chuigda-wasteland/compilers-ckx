#ifndef CKX_CONTEXT_MANAGER_HPP
#define CKX_CONTEXT_MANAGER_HPP

#include "frontend/sema/ckx_context.hpp"
#include "saber/optional.hpp"

namespace ckx
{

class ckx_context_manager
{
    friend class func_context_raii;
public:
    ckx_context_manager();
    ~ckx_context_manager();

    ckx_context_manager(const ckx_context_manager&) = delete;
    ckx_context_manager(ckx_context_manager&&) = delete;

    ckx_func_context* lookup_func_context();
    ckx_if_context* lookup_if_context();
    ckx_while_context* lookup_while_context();

    void enter_func_context(ckx_func_type *_func_type);
    void exit_func_context();

    void enter_if_context(ckx_if_context *_if_context);
    void exit_if_context();

    void enter_while_context(ckx_while_context *_while_context);
    void exit_while_context();

private:
    ckx_context::context_type current_context_type_intern() const;

    saber::vector<ckx_context*> context_chain;
};

} // namespace ckx

#endif // CKX_CONTEXT_MANAGER_HPP
