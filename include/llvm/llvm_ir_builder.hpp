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

#ifndef LLVM_IR_BUILDER_HPP
#define LLVM_IR_BUILDER_HPP

#include "llvm/llvm_inst.hpp"
#include "llvm/llvm_type.hpp"
#include "llvm/llvm_value.hpp"

#include "saber/vector.hpp"
#include "saber/string_pool.hpp"

namespace faker
{

namespace detail
{
class llvm_ir_builder_impl;
} // namespace detail


class llvm_ir_builder final
{
public:
    llvm_ir_builder();
    ~llvm_ir_builder();
    llvm_ir_builder(const llvm_ir_builder&) = delete;
    llvm_ir_builder(llvm_ir_builder&&) = delete;

    llvm_ir_builder& operator= (const llvm_ir_builder&) = delete;
    llvm_ir_builder& operator= (llvm_ir_builder&&) = delete;

    void pretty_print(we::we_file_writer& _writer);

    void create_n_enter_func(llvm_type _return_type, saber_string_view _name,
                             saber::vector<llvm_type> &&_param_type_list,
                             saber::vector<saber_string_view> _param_name_list,
                             llvm_func_attrs _attrs);
    void leave_func();

    llvm_instruction* get_insert_point();
    void set_insert_after(llvm_instruction* _instruction);

    /// @fn control flows
    llvm_instruction* create_func_decl(
            llvm_type _return_type, saber_string_view _name,
            saber::vector<llvm_type> &&_param_type_list,
            saber::vector<saber_string_view> _param_name_list,
            llvm_func_attrs _attrs);
    llvm_instruction* create_return_void();
    llvm_instruction* create_return(llvm_type _type, llvm_value* _value);
    llvm_instruction* create_branch(llvm_label *_label);
    llvm_instruction* create_cond_branch(llvm_value *_cond,
                                         llvm_label *_true_label,
                                         llvm_label *_false_label);
    llvm_instruction* create_phi(llvm_value* _result, llvm_type _type,
                                 llvm_label *_label1, llvm_value *_val1,
                                 llvm_label *_label2, llvm_value *_val2);
    llvm_instruction* create_call(llvm_value* _result, llvm_type _type,
                                  saber_string_view _func_name,
                                  saber::vector<llvm_type> &&_types,
                                  saber::vector<llvm_value*> &&_args);
    llvm_label*       create_label(saber_string_view _name);
    llvm_label*       create_temporary_label();

COMMENT(BEGIN_BLOCK)

    /// @attention We are using macro to write function declarations!
    /// LLVM did so, so we decides to copy its idea!
    /// @fn binary operations, especially calculations
#   define BINOP(OPCODE) \
    llvm_instruction* create_##OPCODE(llvm_value *_result, llvm_type _type,\
                                      llvm_value* _lhs, llvm_value *_rhs);
    /// @fn cast operations
#   define CASTOP(OPCODE) \
    llvm_instruction* create_##OPCODE(llvm_value *_result, llvm_type _src_type,\
                                      llvm_value *_src, llvm_type _dest_type);

    /// @fn comparsions
#   define CMPOP(OPCODE) \
    llvm_instruction* create_##OPCODE(llvm_value *_result, llvm_type _type,\
                                      llvm_value *_val1, llvm_value *_val2);

#   include "llvm/opdef.hpp"
#   undef BINOP
#   undef CASTOP
#   undef CMPOP

COMMENT(END_BLOCK)

    /// @fn memory accessing operations
    llvm_instruction* create_alloca(llvm_value *_result, llvm_type _type,
                                    quint32 _array_size);
    llvm_instruction* create_load(llvm_value *_result, llvm_type _type,
                                  llvm_value *_ptr);
    llvm_instruction* create_store(llvm_type _type,
                                   llvm_value *_src, llvm_value *_result);
    llvm_instruction* create_extractvalue(llvm_value *_result,
                                          llvm_type _type,
                                          llvm_value *_value,
                                          llvm_value *_idx);
    llvm_instruction* create_getelementptr(llvm_value *_result,
                                           llvm_type _type, llvm_value *_ptr,
                                           llvm_type _ty, llvm_value *_idx);
    llvm_instruction* create_getelementptr2(llvm_value *_result,
                                            llvm_type _type, llvm_value *_ptr,
                                            llvm_type _ty1, llvm_value *_idx1,
                                            llvm_type _ty2, llvm_value *_idx2);
    llvm_instruction* create_udt(saber_string_view _name,
                                 saber::vector<llvm_type>&& _fields);

    /// @fn functions dealing with values
    llvm_value *create_string_constant(saber_string_view _str);
    llvm_value *create_signed_constant(qint64 _i);
    llvm_value *create_unsigned_constant(quint64 _u);
    llvm_value *create_floating_constant(qreal _r);
    llvm_value *create_bool_constant(bool _b);
    llvm_value *create_null();


    llvm_value *create_temporary_var();
    llvm_value *create_named_var(saber_string_view _name);

private:
    detail::llvm_ir_builder_impl* impl;
};

} // namespace faker

#endif // LLVM_IR_BUILDER_HPP
