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

namespace faker
{

namespace detail
{
class llvm_ir_builder_impl final
{

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
