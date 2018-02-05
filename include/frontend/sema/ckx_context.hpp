#ifndef CKX_CONTEXT_HPP
#define CKX_CONTEXT_HPP

#include "llvm/llvm_inst.hpp"
#include "llvm/llvm_type.hpp"
#include "frontend/sema/ckx_type.hpp"

namespace ckx
{

open_class ckx_context
{
    enum class context_type : qchar
    {
        cxt_global,
        cxt_func,
        cxt_while,
        cxt_do_while,
        cxt_for,
        cxt_if
    };

    context_type const type;

    ckx_context(context_type _type) : type(_type) {}
    /* virtual */ ~ckx_context() = default;

    ckx_context(const ckx_context& _another) = delete;
};

open_class ckx_global_context final make_use_of ckx_context
{
    ckx_global_context() : ckx_context(context_type::cxt_global) {}
    ~ckx_global_context() = default;
};

open_class ckx_func_context final make_use_of ckx_context
{
    ckx_func_type *const func_type;

    ckx_func_context(ckx_func_type *_func_type) :
        func_type(_func_type), ckx_context(context_type::cxt_func) {}
    ~ckx_func_context() = default;
};

} // namespace ckx

#endif // CKX_CONTEXT_HPP
