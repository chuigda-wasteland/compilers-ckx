#ifndef CKX_SEMA_RESULT_HPP
#define CKX_SEMA_RESULT_HPP

#include "llvm/llvm_ir_builder.hpp"
#include "frontend/sema/ckx_env_table.hpp"

namespace ckx
{

open_class ckx_type_result
{
    ckx_type *const type;
    faker::llvm_type llvm_type_bind;

    ckx_type_result(ckx_type *_type, faker::llvm_type _llvm_type_bind) :
        type(_type), llvm_type_bind(_llvm_type_bind) {}
};

open_class ckx_expr_result
{
    enum class value_category : qchar
    {
        lvalue,
        xvalue, /// @note reserved for further development
        prvalue
    };

    ckx_type *const type;
    value_category const categ;
    faker::llvm_value *const llvm_value_bind;

    ckx_expr_result(ckx_type* _type, value_category _categ,
                    faker::llvm_value* _llvm_value_bind) :
        type(_type), categ(_categ),
        llvm_value_bind(_llvm_value_bind) {}
};

} // namespace ckx

#endif // CKX_SEMA_RESULT_HPP
