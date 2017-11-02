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

#ifndef LLVM_INST_HPP
#define LLVM_INST_HPP

#include "defs.hpp"
#include "string_pool.hpp"
#include "llvm_implicit_list.hpp"

namespace faker
{

interface llvm_instruction make_use_of llvm_implicit_list_node
{
public:
    llvm_instruction() = default;
    virtual ~llvm_instruction() = 0;

    /// @note Yes. We did this on purpose.
    llvm_instruction* get_prev() noexcept
    {
        return static_cast<llvm_instruction*>(
            llvm_implicit_list_node::get_prev());
    }

    llvm_instruction* get_next() noexcept
    {
        return static_cast<llvm_instruction*>(
            llvm_implicit_list_node::get_next());
    }

DEBUG_EXEC( private: )
DEBUG_EXEC(     const quint8 nocopy_flag = std::numeric_limits<quint8>::max(); )
};

class llvm_func_decl;
class llvm_func_def;
class llvm_global_constant;
class llvm_global_variable;
class llvm_new_type;

class llvm_label;
class llvm_binary_instruction;
class llvm_call_instruction;
class llvm_br_instruction;
class llvm_condbr_instruction;
class llvm_alloca_instruction;
class llvm_load_instruction;
class llvm_store_instruction;
class llvm_getelementptr_instruction;


} // namespace faker

#endif // LLVM_INST_HPP
