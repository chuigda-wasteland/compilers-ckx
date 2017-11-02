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

#include "llvm_ir_builder.hpp"
#include "llvm_inst_detail.hpp"

namespace faker
{

namespace detail
{
class llvm_ir_builder_impl final
{
public:
    llvm_ir_builder_impl();
    ~llvm_ir_builder_impl();
    llvm_ir_builder_impl(const llvm_ir_builder_impl&) = delete;
    llvm_ir_builder_impl(llvm_ir_builder_impl&&) = delete;
    llvm_ir_builder_impl& operator= (const llvm_ir_builder_impl&) = delete;
    llvm_ir_builder_impl& operator= (llvm_ir_builder_impl&&) = delete;

    void create_n_enter_func(saber_string_view );
    void leave_func();

    llvm_instruction* get_insert_point();
    void set_insert_after(llvm_instruction* _instruction);

    llvm_ret_instruction* create_return(llvm_type _type, llvm_value* _value);

    llvm_br_instruction* create_branch(llvm_label* _label);

    llvm_condbr_instruction* create_cond_branch(llvm_value *_cond,
                                                llvm_instruction* _true_label,
                                                llvm_instruction* _false_label);

    llvm_phi_instruction* create_phi(llvm_type _type, llvm_value *_rec,
                                     llvm_label *_label1, llvm_value *_val1,
                                     llvm_label *_label2, llvm_value *_val2);

    llvm_call_instruction* create_call(llvm_type _type, llvm_value* _rec,
                                       saber_string_view _callee,
                                       saber::vector<llvm_value*> &&_args);

    llvm_label* create_label(saber_string_view _name);

    llvm_binary_instruction* create_binary_instruction(
            llvm_type _type, llvm_binary_instruction::operator_type _op,
            llvm_value* _rec, llvm_value* _left, llvm_value* _right);

    llvm_cast_instruction* create_cast_instruction(
            llvm_cast_instruction::operator_type _op,
            llvm_value* _rec,
            llvm_type _src_type, llvm_value *_src, llvm_type _desired);

    llvm_alloca_instruction* create_alloca(llvm_type *_type,
                                           quint32 _array_size,
                                           llvm_value *_receiver);

    llvm_load_instruction* create_load(llvm_type *_type,
                                       llvm_value *_ptr,
                                       llvm_value *_rec);
    llvm_store_instruction* create_store(llvm_type _type,
                                         llvm_value *_ptr,
                                         llvm_value *_val);
    llvm_getelementptr_instruction* create_getelementptr(
            llvm_type _type, llvm_value *_ptr, llvm_type _ty, llvm_value *_idx,
            llvm_value *_rec);

    /// More instructions on coming!

private:

};
} // namespace detail

llvm_ir_builder::llvm_ir_builder()
{
    impl = new detail::llvm_ir_builder_impl;
}

llvm_ir_builder::~llvm_ir_builder()
{
    delete impl;
}

} // namespace faker
