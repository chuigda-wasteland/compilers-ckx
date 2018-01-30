#include "frontend/AST/ckx_ast_node.hpp"
#include "frontend/sema/ckx_sema.hpp"

#include <cstdio>

int main()
{
    using namespace ckx;
    ckx_sema_engine engine;

    ckx_prelexed_type type(
        saber::vector<ckx_token>{
            ckx_token(ckx_src_rng::empty(), ckx_token::type::tk_vr32),
            ckx_token(ckx_src_rng::empty(), ckx_token::type::tk_const),
            ckx_token(ckx_src_rng::empty(), ckx_token::type::tk_mul),
            ckx_token(ckx_src_rng::empty(), ckx_token::type::tk_mul)});

    saber::vector<ckx_ast_decl_stmt::init_decl> init_decls;
    init_decls.emplace_back(
        ckx_src_rng::empty(), saber_string_pool::create_view("a"), nullptr);
    init_decls.emplace_back(
        ckx_src_rng::empty(), saber_string_pool::create_view("b"), nullptr);

    ckx_ast_decl_stmt *stmt = new ckx_ast_decl_stmt(
                                  saber::move(type), saber::move(init_decls));

    stmt->accept(engine);
    delete stmt;

    we::we_fp_writer writer(stdout);
    engine.test_print(writer);

    return 0;
}
