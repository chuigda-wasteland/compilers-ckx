#include "llvm/llvm_ir_builder.hpp"
#include <cstdio>

int main()
{
    /// @todo We are about to use `main` function for test
    /// (since I no more want to open individual projects)
    using namespace faker;
    using namespace std;
    using namespace we;

    saber_string_view llvm_i32_type = saber_string_pool::create_view("i32");
    saber_string_view name_a = saber_string_pool::create_view("a");
    saber_string_view name_b = saber_string_pool::create_view("b");
    saber_string_view name_foo = saber_string_pool::create_view("foo");
    saber_string_view name_bar = saber_string_pool::create_view("bar");
    saber_string_view name_baz = saber_string_pool::create_view("baz");
    llvm_func_attrs default_c_attrs =
        llvm_func_attrs(true, llvm_func_attrs::it_default);

    we_fp_writer stdwriter(stdout);

    {
        llvm_ir_builder builder;
        builder.create_n_enter_func(llvm_i32_type,
                                    saber_string_pool::create_view("add"),
                                    saber::vector<llvm_type>{
                                        llvm_i32_type,
                                        llvm_i32_type
                                    },
                                    saber::vector<saber_string_view>{
                                        saber_string_pool::create_view("a"),
                                        saber_string_pool::create_view("b"),
                                    },
                                    default_c_attrs);
        llvm_value *temp_var_1 = builder.create_temporary_var();
        llvm_value *arg_a = builder.create_named_var(
                                saber_string_pool::create_view("a"));
        llvm_value *arg_b = builder.create_named_var(
                                saber_string_pool::create_view("b"));
        builder.create_add(temp_var_1, llvm_i32_type, arg_a, arg_b);
        builder.create_return(llvm_i32_type, temp_var_1);
        builder.leave_func();

        builder.pretty_print(stdwriter);
    }

    {
        llvm_ir_builder builder;

        builder.create_n_enter_func(llvm_i32_type,
                                    name_foo,
                                    saber::vector<llvm_type>{llvm_i32_type},
                                    saber::vector<saber_string_view>{name_bar},
                                    default_c_attrs);
        llvm_value *arg_bar = builder.create_named_var(name_bar);
        llvm_value *zero = builder.create_signed_constant(0);
        llvm_value *temp_cmp_result = builder.create_temporary_var();
        llvm_instruction* cond_expr_inst = builder.create_icmp_slt(
                                               temp_cmp_result,
                                               llvm_i32_type, arg_bar, zero);
        llvm_label * true_label = builder.create_temporary_label();
        builder.create_return(llvm_i32_type, arg_bar);
        llvm_label * false_label = builder.create_temporary_label();
        builder.create_return(llvm_i32_type, zero);
        builder.set_insert_after(cond_expr_inst);
        builder.create_cond_branch(temp_cmp_result, true_label, false_label);
        builder.leave_func();

        builder.pretty_print(stdwriter);
    }
}
