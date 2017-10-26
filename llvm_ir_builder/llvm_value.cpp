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

#include "llvm_value_detail.hpp"

namespace faker
{

llvm_value::~llvm_value() {}


llvm_constant::llvm_constant(qint64 _val) :
    type(constant_type::ct_int)
{
    v.i = _val;
}

llvm_constant::llvm_constant(quint64 _val) :
    type(constant_type::ct_uint)
{
    v.u = _val;
}

llvm_constant::llvm_constant(qreal _val) :
    type(constant_type::ct_real)
{
    v.r = _val;
}

llvm_constant::llvm_constant(decltype(nullptr)) :
    type(constant_type::ct_nullptr)
{}


llvm_global::llvm_global(saber_string_view _name) :
    name(_name)
{}


llvm_variable::llvm_variable(saber_string_view _name) :
    name(_name)
{}


} // namespace faker
