#include "frontend/AST/ckx_ast_node.hpp"
#include "frontend/sema/ckx_sema.hpp"
#include <cstdio>

int main()
{
    using namespace ckx;
    ckx_sema_engine engine;

    saber_string_view ssv_a = saber_string_pool::create_view("a");
    saber_string_view ssv_b = saber_string_pool::create_view("b");
    saber_string_view ssv_demo = saber_string_pool::create_view("demo");

    ckx_prelexed_type vr32_type (
        saber::vector<ckx_token> {
            ckx_token(ckx_src_rng::empty(), ckx_token::type::tk_vr64)
        });

    ckx_prelexed_type composed_type (
        saber::vector<ckx_token>{
            ckx_token(ckx_src_rng::empty(), ckx_token::type::tk_vr32),
            ckx_token(ckx_src_rng::empty(), ckx_token::type::tk_const),
            ckx_token(ckx_src_rng::empty(), ckx_token::type::tk_mul),
            ckx_token(ckx_src_rng::empty(), ckx_token::type::tk_mul)});

    saber::vector<ckx_ast_record_stmt::field_row> rows ;
    rows.emplace_back(
        saber::move(vr32_type),
        saber::vector<ckx_ast_record_stmt::field> {
            ckx_ast_record_stmt::field(ckx_src_rng::empty(), ssv_a),
            ckx_ast_record_stmt::field(ckx_src_rng::empty(), ssv_b)});

    ckx_ast_record_stmt *record =
        new ckx_ast_record_stmt(
            ckx_src_rng::empty(), ckx_src_rng::empty(),
            ckx_src_rng::empty(), ckx_src_rng::empty(),
            ckx_ast_record_stmt::record_tag::rt_struct,
            ssv_demo, saber::move(rows));

    record->accept(engine);

    ckx_prelexed_type vi64_type (
        saber::vector<ckx_token>{
            ckx_token(ckx_src_rng::empty(), ckx_token::type::tk_vi64)
        });

    ckx_prelexed_type demo_type(
            saber::vector<ckx_token> {ckx_token(ckx_src_rng::empty(), ssv_demo)}
        );

    saber::vector<ckx_ast_decl_stmt::init_decl> init_decls;
    init_decls.emplace_back(ckx_src_rng::empty(), ssv_a, nullptr);
    init_decls.emplace_back(ckx_src_rng::empty(), ssv_b, nullptr);

    ckx_ast_decl_stmt *decl =
        new ckx_ast_decl_stmt(saber::move(demo_type), saber::move(init_decls));

    init_decls.emplace_back(ckx_src_rng::empty(), ssv_a, nullptr);
    init_decls.emplace_back(ckx_src_rng::empty(), ssv_b, nullptr);

    engine.test_enter_func();
    decl->accept(engine);
    engine.test_enter_scope();
    ckx_ast_decl_stmt *decl1 =
        new ckx_ast_decl_stmt(saber::move(vi64_type), saber::move(init_decls));
    decl1->accept(engine);
    engine.test_leave_scope();
    engine.test_leave_func();

    we::we_fp_writer writer(stdout);
    engine.test_print(writer);

    delete record;
    delete decl;
    delete decl1;
}
