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

#ifndef LLVM_VALUE_HPP
#define LLVM_VALUE_HPP

#include "defs.hpp"
#include "string_pool.hpp"

namespace faker
{

interface llvm_value
{
public:
    llvm_value() = default;
    virtual ~llvm_value() = 0;
    virtual saber_string_view to_string() = 0;
};

} // namespace faker

#endif // LLVM_VALUE_HPP
