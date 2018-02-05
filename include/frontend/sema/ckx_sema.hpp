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
    void visit_decl_node(ckx_ast_decl_stmt* _decl_stmt);
    void visit_record_node(ckx_ast_record_stmt* _record_stmt);
    void visit_func_node(ckx_ast_func_stmt* _func_stmt);
    void visit_return_node(ckx_ast_return_stmt* _return_stmt);

    ckx_expr_result decay_to_rvalue(ckx_expr_result _expr);
    saber::optional<ckx_expr_result> try_implicit_cast(ckx_expr_result _expr);
    saber::optional<ckx_expr_result> try_static_cast(ckx_expr_result _expr);
    saber::optional<ckx_expr_result> try_const_cast(ckx_expr_result _expr);
    saber::optional<ckx_expr_result> try_reinter_cast(ckx_expr_result _expr);
    saber::optional<ckx_expr_result> try_ckx_cast(ckx_expr_result _expr);

    ckx_expr_result
    visit_vi_literal_node(ckx_ast_vi_literal_expr* _literal_expr);
    ckx_expr_result
    visit_vr_literal_node(ckx_ast_vr_literal_expr* _literal_expr);

    /// @note for test only.
    void test_print(we::we_file_writer& writer);
    void test_enter_func();
    void test_leave_func();
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
        ckx_type_result ret_type_result;
        saber::vector<ckx_type_result> param_types_results;
        saber::vector<saber_string_view> param_names;

        function_header_info(
            ckx_type_result _ret_type_result,
            saber::vector<ckx_type_result> &&_param_type_results,
            saber::vector<saber_string_view> &&_param_names) :
                ret_type_result(_ret_type_result),
                param_types_results(saber::move(_param_type_results)),
                param_names(saber::move(_param_names)) {}

        function_header_info(function_header_info&& _another) :
            function_header_info(_another.ret_type_result,
                                 saber::move(_another.param_types_results),
                                 saber::move(_another.param_names)) {}

        function_header_info(const function_header_info&) = delete;
    };

    saber::optional<function_header_info>
    visit_function_header(ckx_ast_func_stmt *_func_stmt);

    saber::optional<ckx_type_result>
    re_lex_type(const ckx_prelexed_type &_prelexed_type);
    static saber_string map_basic_type(ckx_type::category _basic_type);
    quint64 var_name_mangle_count() { return ++vname_mangle_count; }

    bool is_in_func() const { return in_func; }
    void enter_func();
    void leave_func();
    void enter_scope();
    void leave_scope();
    void error() {}

    ckx_env root_env;
    ckx_env *current_env = &root_env;

    ckx_context_manager context_manager;

    bool in_func = false;
    quint64 vname_mangle_count = 0;

    faker::llvm_ir_builder builder;
};

} // namespace ckx

#endif // CKX_SEMA_HPP
