#ifndef CKX_SEMA_HPP
#define CKX_SEMA_HPP

#include "frontend/AST/ckx_ast_node.hpp"
#include "frontend/parse/ckx_prelexed_type.hpp"
#include "frontend/util/ckx_src_rng.hpp"
#include "frontend/sema/ckx_type.hpp"
#include "frontend/sema/ckx_env_table.hpp"
#include "frontend/sema/ckx_context_manager.hpp"
#include "frontend/sema/ckx_sema_result.hpp"
#include "llvm/llvm_ir_builder.hpp"
#include "saber/optional.hpp"

namespace ckx
{

class ckx_sema_engine
{
public:
    ckx_sema_engine() = default;
    ~ckx_sema_engine() = default;

    ckx_sema_engine(const ckx_sema_engine&) = delete;
    ckx_sema_engine(ckx_sema_engine&&) = delete;

    void visit_translation_unit(ckx_ast_translation_unit *_unit);
    void visit_compound_stmt(ckx_ast_compound_stmt *_stmt);
    void visit_decl_stmt(ckx_ast_decl_stmt* _decl_stmt);
    void visit_record_stmt(ckx_ast_record_stmt* _record_stmt);
    void visit_func_stmt(ckx_ast_func_stmt* _func_stmt);
    void visit_return_stmt(ckx_ast_return_stmt* _return_stmt);

    ckx_expr_result decay_to_rvalue(ckx_expr_result _expr);

    saber::optional<ckx_expr_result>
    try_implicit_cast(ckx_expr_result &_expr, ckx_type *_desired);
    saber::optional<ckx_expr_result>
    try_static_cast(ckx_expr_result _expr, ckx_type *_desired);
    saber::optional<ckx_expr_result>
    try_const_cast(ckx_expr_result _expr, ckx_type *_desired);
    saber::optional<ckx_expr_result>
    try_reinter_cast(ckx_expr_result _expr, ckx_type* _desired);
    saber::optional<ckx_expr_result>
    try_ckx_cast(ckx_expr_result _expr, ckx_type* _desired);

    saber::optional<ckx_expr_result>
    visit_binary_expr(ckx_ast_binary_expr *_binary_expr);
    saber::optional<ckx_expr_result>
    visit_unary_expr(ckx_ast_unary_expr *_unary_expr);
    saber::optional<ckx_expr_result>
    visit_invoke_expr(ckx_ast_invoke_expr *_invoke_expr);
    saber::optional<ckx_expr_result>
    visit_extract_expr(ckx_ast_extract_expr *_extract_expr);
    saber::optional<ckx_expr_result>
    visit_id_expr(ckx_ast_id_expr *_id_expr);
    ckx_expr_result
    visit_vi_literal_node(ckx_ast_vi_literal_expr* _literal_expr);
    ckx_expr_result
    visit_vr_literal_node(ckx_ast_vr_literal_expr* _literal_expr);

    /// @note for test only.
    void test_print(we::we_file_writer& writer);
    void test_enter_scope();
    void test_leave_scope();

private:
    void visit_global_decl(ckx_ast_decl_stmt *_decl_stmt);
    void visit_local_decl(ckx_ast_decl_stmt *_decl_stmt);
    void visit_struct_decl(ckx_ast_record_stmt *_struct_stmt);
    void visit_variant_decl(ckx_ast_record_stmt *_variant_stmt);
    void visit_func_decl(ckx_ast_func_stmt *_func_stmt);
    void visit_func_def(ckx_ast_func_stmt *_func_stmt);

    struct function_header_info
    {
        ckx_type* ret_type;
        saber::vector<ckx_type*> param_types;
        saber::vector<saber_string_view> param_names;

        function_header_info(
            ckx_type* _ret_type,
            saber::vector<ckx_type*> &&_param_types,
            saber::vector<saber_string_view> &&_param_names) :
                ret_type(_ret_type),
                param_types(saber::move(_param_types)),
                param_names(saber::move(_param_names)) {}

        function_header_info(function_header_info&& _another) :
            function_header_info(_another.ret_type,
                                 saber::move(_another.param_types),
                                 saber::move(_another.param_names)) {}

        function_header_info(const function_header_info&) = delete;
    };

    saber::optional<function_header_info>
    visit_function_header(ckx_ast_func_stmt *_func_stmt);

    saber::optional<ckx_expr_result>
    visit_assign_expr(ckx_ast_binary_expr *_assign_expr);

    saber::optional<ckx_expr_result>
    visit_addressof_expr(ckx_ast_unary_expr *_unary_expr);

    saber::optional<ckx_expr_result>
    visit_deref_expr(ckx_ast_unary_expr *_unary_expr);

    quint64 calculate_disagreements(const saber::vector<ckx_expr_result>& _args,
                                    const saber::vector<ckx_type*>& _params);

    ckx_type*
    re_lex_type(const ckx_prelexed_type &_prelexed_type);

    quint64 var_name_mangle_count() { return ++vname_mangle_count; }

    class enter_func_protection_raii
    {
    public:
        enter_func_protection_raii(ckx_sema_engine& _sema,
                                   ckx_func_type* _func_type);

        ~enter_func_protection_raii();

    private:
        ckx_sema_engine& sema;
    };

    bool is_in_func();
    void enter_scope();
    void leave_scope();
    void error() { std::printf("fuck!\n"); abort(); }

    ckx_env root_env;
    ckx_env *current_env = &root_env;

    ckx_context_manager context_manager;

    bool in_func = false;
    quint64 vname_mangle_count = 0;

    faker::llvm_ir_builder builder;
};

} // namespace ckx

#endif // CKX_SEMA_HPP
