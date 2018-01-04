#ifndef CKX_SEMA_RESULT_HPP
#define CKX_SEMA_RESULT_HPP

#include "../llvm_ir_builder/llvm_ir_builder.hpp"
#include "ckx_env_table.hpp"

#include "ckx_sema_result_fwd.hpp"

namespace ckx
{

open_class ckx_expr_result
{
    enum class value_category : qchar
    {
        lvalue,
        xvalue, /// @note reserved for further development
        prvalue
    };

    const ckx_type *type;
    const value_category categ;
    const faker::llvm_value *llvm_value_bind;

    ckx_expr_result(const ckx_type* _type, const value_category _categ,
                    const faker::llvm_value* _llvm_value_bind) :
        type(_type), categ(_categ),
        llvm_value_bind(_llvm_value_bind) {}
};

static_assert(sizeof(ckx_expr_result) <= sizeof(ckx_expr_result_storage), "");

} // namespace ckx

#endif // CKX_SEMA_RESULT_HPP
