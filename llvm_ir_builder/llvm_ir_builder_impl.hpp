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

#ifndef LLVM_IR_BUILDER_IMPL_HPP
#define LLVM_IR_BUILDER_IMPL_HPP

#include "llvm_inst_detail.hpp"
#include "llvm_value.hpp"
#include "llvm_type.hpp"
#include "vector.hpp"
#include "list.hpp"

namespace faker
{
namespace detail
{

class llvm_ir_builder_impl final
{
public:
    llvm_ir_builder_impl() = default;
    ~llvm_ir_builder_impl();
    llvm_ir_builder_impl(const llvm_ir_builder_impl&) = delete;
    llvm_ir_builder_impl(llvm_ir_builder_impl&&) = delete;
    llvm_ir_builder_impl& operator= (const llvm_ir_builder_impl&) = delete;
    llvm_ir_builder_impl& operator= (llvm_ir_builder_impl&&) = delete;

    void create_n_enter_func(llvm_type _return_type, saber_string_view _name,
                             saber::vector<llvm_type>&& _param_type_list,
                             saber::vector<saber_string_view> _param_name_list,
                             llvm_func_attrs _attrs);
    void leave_func();

    llvm_instruction* get_insert_point();
    void set_insert_after(llvm_instruction* _instruction);

    /// @note all `create` functions here mean `create and insert after current`
    llvm_func_decl* create_func_decl(
            llvm_type _return_type, saber_string_view _name,
            saber::vector<llvm_type>&& _param_type_list,
            saber::vector<saber_string_view> _param_name_list,
            llvm_func_attrs _attrs);

    llvm_ret_instruction* create_return(llvm_type _type, llvm_value* _value);

    llvm_br_instruction* create_branch(llvm_label* _label);

    llvm_condbr_instruction* create_cond_branch(llvm_value *_cond,
                                                llvm_label *_true_label,
                                                llvm_label *_false_label);

    llvm_phi_instruction* create_phi(llvm_value *_result, llvm_type _type,
                                     llvm_label *_label1, llvm_value *_val1,
                                     llvm_label *_label2, llvm_value *_val2);

    llvm_call_instruction* create_call(llvm_value* _result, llvm_type _type,
                                       saber_string_view _func_name,
                                       saber::vector<llvm_value*> &&_args);

    llvm_label* create_label(saber_string_view _name);
    llvm_label* create_temporary_label();

    llvm_binary_instruction* create_binary_instruction(
            llvm_value* _result,
            llvm_binary_instruction::operator_type _op,
            llvm_type _type,
            llvm_value* _lhs, llvm_value* _rhs);

    llvm_cast_instruction* create_cast_instruction(
            llvm_value* _result, llvm_cast_instruction::operator_type _op,
            llvm_type _src_type, llvm_value *_value, llvm_type _dest_type);

    llvm_cmp_instruction* create_cmp_instruction(
            llvm_value* _result, llvm_cmp_instruction::comparsion_type _op,
            llvm_type _compared_type, llvm_value *_val1, llvm_value *_val2);

    llvm_alloca_instruction* create_alloca(llvm_value *_result,
                                           llvm_type _yield_type,
                                           quint32 _num_elems = 1);

    llvm_load_instruction* create_load(llvm_value *_result,
                                       llvm_type _yield_type,
                                       llvm_value *_ptr);

    llvm_store_instruction* create_store(llvm_type _src_type,
                                         llvm_value *_src,
                                         llvm_value *_result);

    llvm_getelementptr_instruction* create_getelementptr(llvm_value *_result,
                                                         llvm_type _yield_type,
                                                         llvm_value* _ptr,
                                                         llvm_type _ty,
                                                         llvm_value* _idx);

    llvm_value *create_string_constant(saber_string_view _str);
    llvm_value *create_signed_constant(qint64 _i);
    llvm_value *create_unsigned_constant(quint64 _u);
    llvm_value *create_floating_constant(qreal _r);
    llvm_value *create_bool_constant(bool _b);

    llvm_value *create_temporary_var();
    llvm_value *create_named_var(saber_string_view _name);

private:
    open_class llvm_function_block /// `Type rich programming`?
    {
        llvm_function_block() = delete;
        llvm_function_block(const llvm_function_block&) = delete;
        llvm_function_block(llvm_function_block&&) = delete;

        llvm_function_block(llvm_func_def* _fndef) : fndef(_fndef) {}
        ~llvm_function_block() { delete fndef; }
        llvm_func_def* fndef;
    };

    template <typename InstructionPtrType>
    InstructionPtrType insert_after_current(InstructionPtrType _instruction)
    {
        _instruction->insert_self_after(cur_insertion_point);
        cur_insertion_point = _instruction;
        return _instruction;
    }

    llvm_value* insert_into_table(llvm_value* _value) {
        value_table.push_back(_value);
        return _value;
    }

    saber::list<llvm_function_block*> functions;
    llvm_implicit_list_node global_head_node;
    llvm_implicit_list_node *stashed_global_insertion_point = nullptr;
    llvm_implicit_list_node *cur_insertion_point = &global_head_node;

    quint64 local_temp_var_counter = 0;
    quint64 local_temp_label_counter = 0;

    saber::list<llvm_value*> value_table;
};

} // namespace detail
} // namespace faker

#endif // LLVM_IR_BUILDER_IMPL_HPP
