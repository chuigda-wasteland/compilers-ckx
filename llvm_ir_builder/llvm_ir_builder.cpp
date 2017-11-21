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
#include "llvm_ir_builder_impl.hpp"

namespace faker
{

llvm_ir_builder::llvm_ir_builder()
{
    impl = new detail::llvm_ir_builder_impl;
}

llvm_ir_builder::~llvm_ir_builder()
{
    delete impl;
}



llvm_instruction*
llvm_ir_builder::create_return(llvm_type _type, llvm_value *_value)
{
    return impl->create_return(_type, _value);
}

llvm_instruction*
llvm_ir_builder::create_return_void()
{
    return impl->create_return(saber_string_pool::create_view("void"), nullptr);
}

llvm_instruction*
llvm_ir_builder::create_branch(llvm_label *_label)
{
    return impl->create_branch(_label);
}

llvm_instruction*
llvm_ir_builder::create_cond_branch(llvm_value *_cond,
                                    llvm_instruction *_true_label,
                                    llvm_instruction *_false_label)
{
    return impl->create_cond_branch(_cond, _true_label, _false_label);
}

llvm_instruction*
llvm_ir_builder::create_phi(llvm_value *_result, llvm_type _type,
                            llvm_label *_label1, llvm_value *_val1,
                            llvm_label *_label2, llvm_value *_val2)
{
    return impl->create_phi(_result, _type, _label1, _val1, _label2, _val2);
}

llvm_instruction*
llvm_ir_builder::create_call(llvm_value *_result, llvm_type _type,
                             saber_string_view _func_name,
                             saber::vector<llvm_value *> &&_args)
{
    return impl->create_call(_result, _type, _func_name, saber::move(_args));
}

llvm_label*
llvm_ir_builder::create_label(saber_string_view _name)
{
    return impl->create_label(_name);
}


COMMENT(BEGIN_BLOCK)
#   define BINOP(OPCODE) \
    llvm_instruction* \
    llvm_ir_builder::create_##OPCODE(llvm_value *_result, llvm_type _type,\
                                     llvm_value* _lhs, llvm_value *_rhs) \
    { \
        return impl->create_binary_instruction( \
            _result, llvm_binary_instruction::operator_type::ot_##OPCODE, \
            _type, _lhs, _rhs); \
    }

#   define CASTOP(OPCODE) \
    llvm_instruction* \
    llvm_ir_builder::create_##OPCODE(llvm_value *_result, llvm_type _src_type,\
                                     llvm_value *_src, llvm_type _dest_type) \
    { \
        return impl->create_cast_instruction( \
             _result, llvm_cast_instruction::operator_type::ot_##OPCODE, \
            _src_type, _src, _dest_type); \
    }

#   define CMPOP(OPCODE) \
    llvm_instruction* \
    llvm_ir_builder::create_##OPCODE(llvm_value *_result, llvm_type _type,\
                                     llvm_value *_val1, llvm_value *_val2) \
    { \
        return impl->create_cmp_instruction( \
            _result, llvm_cmp_instruction::comparsion_type::ot_##OPCODE, \
            _type, _val1, _val2); \
    }

#   include "opdef.hpp"
#   undef BINOP
#   undef CASTOP
#   undef CMPOP
COMMENT(END_BLOCK)


llvm_instruction*
llvm_ir_builder::create_alloca(llvm_value *_result,
                               llvm_type _type, quint32 _array_size)
{
    return impl->create_alloca(_result, _type, _array_size);
}

llvm_instruction*
llvm_ir_builder::create_load(llvm_value *_result,
                             llvm_type _type, llvm_value *_ptr)
{
    return impl->create_load(_result, _type, _ptr);
}

llvm_instruction*
llvm_ir_builder::create_store(llvm_type _type, llvm_value *_ptr,
                              llvm_value *_val)
{
    return impl->create_store(_type, _ptr, _val);
}

llvm_instruction*
llvm_ir_builder::create_getelementptr(llvm_value *_result,
                                      llvm_type _type, llvm_value *_ptr,
                                      llvm_type _ty, llvm_value *_idx)
{
    return impl->create_getelementptr(_result, _type, _ptr, _ty, _idx);
}

llvm_value *llvm_ir_builder::create_string_constant(saber_string_view _str)
{
    return impl->create_string_constant(_str);
}

llvm_value *llvm_ir_builder::create_signed_constant(qint64 _i)
{
    return impl->create_signed_constant(_i);
}

llvm_value *llvm_ir_builder::create_unsigned_constant(quint64 _u)
{
    return impl->create_unsigned_constant(_u);
}

llvm_value *llvm_ir_builder::create_floating_constant(qreal _r)
{
    return impl->create_floating_constant(_r);
}

llvm_value *llvm_ir_builder::create_bool_constant(bool _b)
{
    return impl->create_bool_constant(_b);
}

llvm_value *llvm_ir_builder::create_temporary_var()
{
    return impl->create_temporary_var();
}

llvm_value *llvm_ir_builder::create_named_var(saber_string_view _name)
{
    return impl->create_named_var(_name);
}


} // namespace faker

