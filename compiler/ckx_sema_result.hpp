#ifndef CKX_SEMA_RESULT_HPP
#define CKX_SEMA_RESULT_HPP

#include "../llvm_ir_builder/llvm_ir_builder.hpp"
#include "ckx_env_table.hpp"

namespace ckx
{

open_class ckx_translation_result
{
    ckx_translation_result() = default;
    ~ckx_translation_result() = default;

    ckx_translation_result(const ckx_translation_result&) = delete;
    ckx_translation_result(ckx_translation_result&&) {}
};

open_class ckx_expr_result make_use_of ckx_translation_result
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

    ckx_expr_result(const ckx_type* _type, const value_category* _categ,
                    const faker::llvm_value* _llvm_value_bind) :
        type(_type), categ(_categ),
        llvm_value_bind(_llvm_value_bind) {}
};

open_class ckx_decl_result make_use_of ckx_translation_result
{
    const ckx_env_var_entry *entry_bind;
    const ckx_expr_result *id_expr;

    ckx_decl_result(const ckx_env_var_entry* _entry_bind,
                    const ckx_expr_result* _id_expr) :
        entry_bind(_entry_bind), id_expr(_id_expr) {}
};

open_class ckx_func_result make_use_of ckx_translation_result
{
    const ckx_env_func_entry *entry_bind;

    ckx_func_result(const ckx_env_func_entry *_entry_bind) :
        entry_bind(_entry_bind) {}
};

open_class ckx_type_result make_use_of ckx_translation_result
{
    const ckx_env_type_entry *entry_bind;

    ckx_type_result(const ckx_env_type_entry _entry_bind) :
        entry_bind(_entry_bind) {}
};

open_class ckx_stmt_result make_use_of ckx_translation_result
{
    const faker::llvm_instruction *stmt_bind_begin;
    const faker::llvm_instruction *stmt_bind_end;

    ckx_stmt_result(const faker::llvm_instruction *_stmt_bind_begin,
                    const faker::llvm_instruction *_stmt_bind_end) :
        stmt_bind_begin(_stmt_bind_begin),
        stmt_bind_end(_stmt_bind_end) {}
};

} // namespace ckx

#endif // CKX_SEMA_RESULT_HPP
