#include "llvm_ir_builder.hpp"
#include <cstdio>

int main()
{
    /// @todo We are about to use `main` function for test
    /// (since I no more want to open individual projects)
    using namespace faker;
    using namespace std;
    using namespace we;

    llvm_ir_builder builder;
    builder.create_func_decl(saber_string_pool::create_view("i32"),
                             saber_string_pool::create_view("nothing"),
                             saber::vector<llvm_type>(),
                             saber::vector<saber_string_view>(),
                             llvm_func_attrs(true,
                                             llvm_func_attrs::it_default)
                             );
    builder.create_n_enter_func(saber_string_pool::create_view("i8"),
                                saber_string_pool::create_view("main"),
                                saber::vector<llvm_type>(),
                                saber::vector<saber_string_view>(),
                                llvm_func_attrs(true,
                                                llvm_func_attrs::it_default));
    llvm_value *value_a = builder.create_named_var(
                              saber_string_pool::create_view("a1"));
    builder.create_alloca(value_a, saber_string_pool::create_view("i32"), 4);
    llvm_value *temp1 = builder.create_temporary_var();
    builder.create_call(temp1,
                        saber_string_pool::create_view("i32"),
                        saber_string_pool::create_view("nothing"),
                        saber::vector<llvm_value*>());
    builder.create_store(saber_string_pool::create_view("i32"), temp1, value_a);
    builder.create_return(saber_string_pool::create_view("i8"),
                          builder.create_signed_constant(0));
    builder.leave_func();

    we_fp_writer writer(stdout);
    builder.pretty_print(writer);
}
