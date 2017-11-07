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

#ifndef LLVM_VALUE_DETAIL_HPP
#define LLVM_VALUE_DETAIL_HPP

#include "llvm_value.hpp"

namespace faker
{

class llvm_constant final implements llvm_value
{
public:
    llvm_constant() = delete;
    llvm_constant(const llvm_constant& _another) = delete;

    llvm_constant(qint64 _val);
    llvm_constant(quint64 _val);
    llvm_constant(qreal _val);
    llvm_constant(decltype(nullptr));
    ~llvm_constant() override final = default;

    saber_string_view to_string() override final;

private:
    enum class constant_type { ct_int, ct_uint, ct_real, ct_nullptr } type;
    variant { qint64 i; quint64 u; qreal r; } v;
};


class llvm_global final implements llvm_value
{
public:
    llvm_global() = delete;
    llvm_global(const llvm_global& _another) = delete;
    llvm_global(saber_string_view _name);
    ~llvm_global() override final = default;

    saber_string_view to_string() override final;

private:
    saber_string_view name;
};


class llvm_variable final implements llvm_value
{
public:
    llvm_variable() = delete;
    llvm_variable(const llvm_variable& _another) = delete;
    llvm_variable(saber_string_view _name);
    ~llvm_variable() override final = default;

    saber_string_view to_string() override final;

private:
    saber_string_view name;
};

} // namespace faker

#endif // LLVM_VALUE_DETAIL_HPP
