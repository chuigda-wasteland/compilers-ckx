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

#include "llvm_inst_detail.hpp"

namespace faker
{

llvm_func_decl::llvm_func_decl(
        saber_string_view _name,
        saber::vector<llvm_type> &&_param_type_list,
        saber::vector<saber_string_view> &&_param_name_list,
        llvm_type _return_type) :
    name(_name),
    param_type_list(saber::move(_param_type_list)),
    param_name_list(saber::move(_param_name_list)),
    return_type(_return_type)
{}


llvm_func_def::llvm_func_def(
        saber_string_view _name,
        saber::vector<llvm_type> &&_param_type_list,
        saber::vector<saber_string_view> &&_param_name_list,
        llvm_type _return_type) :
    name(_name),
    param_type_list(saber::move(_param_type_list)),
    param_name_list(saber::move(_param_name_list)),
    return_type(_return_type)
{}


llvm_global_constant::llvm_global_constant(llvm_value *_value) :
    value(_value)
{}


llvm_global_variable::llvm_global_variable(llvm_type _type,
                                           saber_string_view _name) :
    type(_type),
    name(_name)
{}


llvm_new_type::llvm_new_type(saber_string_view _name,
                             saber::vector<llvm_type> &&_fields) :
    name(_name),
    fields(saber::move(_fields))
{}


void llvm_label::set_label_name(saber_string_view _name)
{
    DEBUG_EXEC( assert(!name_set) );
    DEBUG_EXEC( name_set = true; );
    name = _name;
}


saber_string_view llvm_label::get_label_name()
{
    DEBUG_EXEC( assert(name_set) );
    return name;
}


llvm_binary_instruction::llvm_binary_instruction(llvm_value *_result,
                                                 operator_type _op,
                                                 llvm_type _yield_type,
                                                 llvm_value *_lhs,
                                                 llvm_value *_rhs) :
    result(_result),
    op(_op),
    yield_type(_yield_type),
    lhs(_lhs),
    rhs(_rhs)
{}


llvm_call_instruction::llvm_call_instruction(
        saber_string_view _func_name,
        llvm_type _yield_type,
        saber::vector<llvm_value *> &&_args,
        llvm_value *_result) :
    func_name(_func_name),
    yield_type(_yield_type),
    args(saber::move(_args)),
    result(_result)
{}


llvm_br_instruction::llvm_br_instruction(llvm_label *_dest) :
    dest(_dest)
{}


llvm_condbr_instruction::llvm_condbr_instruction(llvm_value *_condition,
                                                 llvm_label *_true_label,
                                                 llvm_label *_false_label) :
    condition(_condition),
    true_label(_true_label),
    false_label(_false_label)
{}


llvm_alloca_instruction::llvm_alloca_instruction(llvm_value *_result,
                                                 llvm_type _yield_type,
                                                 quint32 _num_elems) :
    result(_result),
    yield_type(_yield_type),
    num_elems(_num_elems)
{}


llvm_load_instruction::llvm_load_instruction(llvm_value *_result,
                                             llvm_type _yield_type,
                                             llvm_value *_ptr) :
    result(_result),
    yield_type(_yield_type),
    ptr(_ptr)
{}


llvm_store_instruction::llvm_store_instruction(llvm_type _src_type,
                                               llvm_value *_src,
                                               llvm_value *_result) :
    src_type(_src_type),
    src(_src),
    result(_result)
{}


llvm_getelementptr_instruction::llvm_getelementptr_instruction(
        llvm_value *_result,
        llvm_type _yield_type,
        llvm_value *_ptr,
        llvm_type _ty,
        llvm_value *_idx) :
    result(_result),
    yield_type(_yield_type),
    ptr(_ptr),
    ty(_ty),
    idx(_idx)
{}

} // namespace faker
