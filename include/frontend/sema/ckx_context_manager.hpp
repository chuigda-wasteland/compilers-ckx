#ifndef CKX_CONTEXT_MANAGER_HPP
#define CKX_CONTEXT_MANAGER_HPP

#include "frontend/sema/ckx_context.hpp"
#include "saber/list.hpp"

namespace ckx
{

class ckx_context_manager
{
    friend class func_context_raii;
public:
    ckx_context_manager();
    ~ckx_context_manager() = default;

    ckx_context_manager(const ckx_context_manager&) = delete;
    ckx_context_manager(ckx_context_manager&&) = delete;

    class func_context_raii
    {
    public:
        func_context_raii(ckx_context_manager& _context_manager,
                          ckx_func_type *_func_type);

        ~func_context_raii();

        func_context_raii(const func_context_raii&) = delete;
        func_context_raii(func_context_raii&&) = delete;

    private:
        ckx_context_manager& context_manager;
    };

private:
    ckx_context::context_type current_context_type_intern() const;
    void enter_func_context_intern(ckx_func_type *_func_type);
    void exit_func_context_intern();

    saber::list<ckx_context> context_chain;
};

} // namespace ckx

#endif // CKX_CONTEXT_MANAGER_HPP
