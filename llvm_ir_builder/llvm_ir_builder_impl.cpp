/**
    The Opensource Compiler CKX -- for my honey ChenKX
    [[ The self-written "Faker" LLVM Builder part ]]
    Copyright (C) 2017  CousinZe

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/.
  */

#include "llvm_ir_builder_impl.hpp"
#include "llvm_value_detail.hpp"

namespace faker
{
namespace detail
{

llvm_ir_builder_impl::~llvm_ir_builder_impl()
{
    for (llvm_value* value : value_table)
        delete value;

    for (llvm_function_block* function : functions)
        delete function;

    llvm_instruction *iter = static_cast<llvm_instruction*>(
                                 global_head_node.get_next());

    while (iter != nullptr)
    {
        llvm_instruction* to_be_deleted = iter;
        iter = iter->get_next();
        delete to_be_deleted;
    }
}

void llvm_ir_builder_impl::pretty_print(we::we_file_writer &_writer)
{
    for (llvm_function_block* function : functions)
        function->fndef->pretty_print(_writer);

    llvm_instruction* iter =
        static_cast<llvm_instruction*>(global_head_node.get_next());
    while (iter != nullptr)
    {
        iter->pretty_print(_writer);
        iter = iter->get_next();
    }
}

void llvm_ir_builder_impl::create_n_enter_func(
        llvm_type _return_type,
        saber_string_view _name,
        saber::vector<llvm_type> &&_param_type_list,
        saber::vector<saber_string_view> _param_name_list,
        llvm_func_attrs _attrs)
{
    assert(local_temp_var_counter == 0);
    assert(local_temp_label_counter == 0);
    assert(stashed_global_insertion_point == nullptr);
    auto fndef = new llvm_func_def(_return_type, _name,
                                   saber::move(_param_type_list),
                                   saber::move(_param_name_list),
                                   _attrs);
    functions.push_back(new llvm_function_block(fndef));
    stashed_global_insertion_point = cur_insertion_point;
    cur_insertion_point = fndef;
}

void llvm_ir_builder_impl::leave_func()
{
    local_temp_var_counter = 0;
    local_temp_label_counter = 0;

    cur_insertion_point = stashed_global_insertion_point;
    stashed_global_insertion_point = nullptr;
}

void llvm_ir_builder_impl::set_insert_after(llvm_instruction *_instruction)
{
    cur_insertion_point = _instruction;
}

llvm_func_decl *llvm_ir_builder_impl::create_func_decl(
        llvm_type _return_type, saber_string_view _name,
        saber::vector<llvm_type> &&_param_type_list,
        saber::vector<saber_string_view> _param_name_list,
        llvm_func_attrs _attrs)
{
    return insert_after_current(
        new llvm_func_decl(
            _return_type, _name,
            saber::move(_param_type_list), saber::move(_param_name_list),
            _attrs));
}

llvm_ret_instruction* llvm_ir_builder_impl::create_return(llvm_type _type,
                                                          llvm_value *_value)
{
    return insert_after_current(new llvm_ret_instruction(_type, _value));
}

llvm_br_instruction* llvm_ir_builder_impl::create_branch(llvm_label *_label)
{
    return insert_after_current(new llvm_br_instruction(_label));
}

llvm_condbr_instruction* llvm_ir_builder_impl::create_cond_branch(
        llvm_value *_cond, llvm_label *_true_label, llvm_label *_false_label)
{
    return insert_after_current(
        new llvm_condbr_instruction(_cond, _true_label, _false_label));
}

llvm_phi_instruction* llvm_ir_builder_impl::create_phi(
        llvm_value *_result, llvm_type _type,
        llvm_label *_label1, llvm_value *_val1,
        llvm_label *_label2, llvm_value *_val2)
{
    return insert_after_current(
        new llvm_phi_instruction(
            _result, _type, _label1, _val1, _label2, _val2));
}

llvm_call_instruction* llvm_ir_builder_impl::create_call(
        llvm_value *_result, llvm_type _type,
        saber_string_view _func_name, saber::vector<llvm_value*> &&_args)
{
    return insert_after_current(
        new llvm_call_instruction(
            _result, _type, _func_name, saber::move(_args)));
}

llvm_label* llvm_ir_builder_impl::create_label(saber_string_view _name)
{
    return insert_after_current(new llvm_label(_name));
}

llvm_label* llvm_ir_builder_impl::create_temporary_label()
{
    return insert_after_current(
        new llvm_label(
            saber_string_pool::create_view(
                saber::string_paste("tl.", local_temp_label_counter++))));
}

llvm_binary_instruction* llvm_ir_builder_impl::create_binary_instruction(
        llvm_value *_result, llvm_binary_instruction::operator_type _op,
        llvm_type _type, llvm_value *_lhs, llvm_value *_rhs)
{
    return insert_after_current(
        new llvm_binary_instruction(_result, _op, _type, _lhs, _rhs));
}

llvm_cast_instruction* llvm_ir_builder_impl::create_cast_instruction(
        llvm_value *_result, llvm_cast_instruction::operator_type _op,
        llvm_type _src_type, llvm_value *_value, llvm_type _dest_type)
{
    return insert_after_current(
        new llvm_cast_instruction(_result, _op, _src_type, _value, _dest_type));
}

llvm_cmp_instruction* llvm_ir_builder_impl::create_cmp_instruction(
        llvm_value *_result, llvm_cmp_instruction::comparsion_type _op,
        llvm_type _compared_type, llvm_value *_val1, llvm_value *_val2)
{
    return insert_after_current(
        new llvm_cmp_instruction(_result, _op, _compared_type, _val1, _val2));
}

llvm_alloca_instruction* llvm_ir_builder_impl::create_alloca(
        llvm_value *_result, llvm_type _yield_type, quint32 _num_elems)
{
    return insert_after_current(
        new llvm_alloca_instruction(_result, _yield_type, _num_elems));
}

llvm_load_instruction* llvm_ir_builder_impl::create_load(
        llvm_value *_result, llvm_type _yield_type, llvm_value *_ptr)
{
    return insert_after_current(
        new llvm_load_instruction(_result, _yield_type, _ptr));
}

llvm_store_instruction* llvm_ir_builder_impl::create_store(
        llvm_type _src_type, llvm_value *_src, llvm_value *_result)
{
    return insert_after_current(
        new llvm_store_instruction(_src_type, _src, _result));
}

llvm_getelementptr_instruction* llvm_ir_builder_impl::create_getelementptr(
        llvm_value *_result, llvm_type _yield_type, llvm_value *_ptr,
        llvm_type _ty, llvm_value *_idx)
{
    return insert_after_current(
        new llvm_getelementptr_instruction(
            _result, _yield_type, _ptr, _ty, _idx));
}

llvm_value* llvm_ir_builder_impl::create_string_constant(saber_string_view _str)
{
    return insert_into_table(new llvm_constant(_str));
}

llvm_value* llvm_ir_builder_impl::create_signed_constant(qint64 _i)
{
    return insert_into_table(new llvm_constant(_i));
}

llvm_value* llvm_ir_builder_impl::create_unsigned_constant(quint64 _u)
{
    return insert_into_table(new llvm_constant(_u));
}

llvm_value* llvm_ir_builder_impl::create_floating_constant(qreal _r)
{
    return insert_into_table(new llvm_constant(_r));
}

llvm_value* llvm_ir_builder_impl::create_bool_constant(bool _b)
{
    return insert_into_table(new llvm_constant(_b));
}

llvm_value* llvm_ir_builder_impl::create_temporary_var()
{
    return insert_into_table(
        new llvm_variable(
            saber_string_pool::create_view(
                saber::string_paste("tv.", local_temp_var_counter++))));
}

llvm_value* llvm_ir_builder_impl::create_named_var(saber_string_view _name)
{
    return insert_into_table(new llvm_variable(_name));
}

} // namespace detail
} // namespace faker
