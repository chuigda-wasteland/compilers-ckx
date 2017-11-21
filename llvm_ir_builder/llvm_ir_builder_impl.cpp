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
    /// Incomplete yet. needs more refining.
}

llvm_binary_instruction* llvm_ir_builder_impl::create_binary_instruction(
        llvm_value *_result, llvm_binary_instruction::operator_type _op,
        llvm_type _type, llvm_value *_lhs, llvm_value *_rhs)
{
    return insert_after_current(
        new llvm_binary_instruction(_result, _op, _type, _lhs, _rhs));
}

/// @todo add string support for llvm_constant first
/*
llvm_value* llvm_ir_builder_impl::create_string_constant(saber_string_view _str)
{
    return insert_into_table(new llvm_constant(_str));
}
*/

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

/// @todo add bool support for llvm_constant first
/*
llvm_value* llvm_ir_builder_impl::create_bool_constant(bool _b)
{
    return insert_into_table(new llvm_constant(_b));
}
*/

llvm_value* llvm_ir_builder_impl::create_temporary_var()
{
    return insert_into_table(
        new llvm_variable(
            saber_string_pool::create_view(
                saber::to_string_helper(local_temp_var_counter))));
}

llvm_value* llvm_ir_builder_impl::create_named_var(saber_string_view _name)
{
    return insert_into_table(new llvm_variable(_name));
}

} // namespace detail
} // namespace faker
