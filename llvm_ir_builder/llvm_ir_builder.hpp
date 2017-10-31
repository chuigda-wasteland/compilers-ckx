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

#include "llvm_inst.hpp"
#include "llvm_type.hpp"
#include "llvm_value.hpp"

#include "vector.hpp"
#include "string_pool.hpp"

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

    void create_n_enter_func(saber_string_view _name, llvm_type _ret);
    void leave_func();

    llvm_instruction* get_insert_point();
    void set_insert_point(llvm_instruction* _instruction);

    /// @fn control flows
    llvm_instruction* create_return_void();
    llvm_instruction* create_return(llvm_type _type, llvm_value* _value);
    llvm_instruction* create_branch(llvm_instruction* _label);
    llvm_instruction* create_cond_branch(llvm_value *_cond,
                                         llvm_instruction* _true_label,
                                         llvm_instruction* _false_label);
    llvm_instruction* create_phi(llvm_type _type, llvm_value* _rec,
                                 llvm_instruction* _label1, llvm_value *_val1,
                                 llvm_instruction* _label2, llvm_value *_val2);
    llvm_instruction* create_call(llvm_type _type, llvm_value* _rec,
                                  saber_string_view _callee,
                                  saber::vector<llvm_value*> &&_args);
    llvm_instruction* create_label(saber_string_view _name);
    llvm_instruction* create_unnamed_label();

    /// @fn binary operations, especially calculations
    llvm_instruction* create_add(llvm_type _type, llvm_value *_left,
                                 llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_sub(llvm_type _type, llvm_value *_left,
                                 llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_mul(llvm_type _type, llvm_value *_left,
                                 llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_sdiv(llvm_type _type, llvm_value *_left,
                                  llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_udiv(llvm_type _type, llvm_value *_left,
                                  llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_srem(llvm_type _type, llvm_value *_left,
                                  llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_urem(llvm_type _type, llvm_value *_left,
                                  llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_fadd(llvm_type _type, llvm_value *_left,
                                  llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_fsub(llvm_type _type, llvm_value *_left,
                                  llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_fmul(llvm_type _type, llvm_value *_left,
                                  llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_fdiv(llvm_type _type, llvm_value *_left,
                                  llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_shl(llvm_type _type, llvm_value *_left,
                                 llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_lshr(llvm_type _type, llvm_value *_left,
                                  llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_ashr(llvm_type _type, llvm_value *_left,
                                  llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_and(llvm_type _type, llvm_value *_left,
                                 llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_or(llvm_type _type, llvm_value *_left,
                                llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_xor(llvm_type _type, llvm_value *_left,
                                 llvm_value *_right, llvm_value *_rec);

    /// @fn cast operations
    llvm_instruction* create_trunc(llvm_type _src_type, llvm_value *_src,
                                   llvm_type _desired, llvm_value *_rec);
    llvm_instruction* create_zext(llvm_type _src_type, llvm_value *_src,
                                   llvm_type _desired, llvm_value *_rec);
    llvm_instruction* create_sext(llvm_type _src_type, llvm_value *_src,
                                  llvm_type _desired, llvm_value *_rec);
    llvm_instruction* create_fptoui(llvm_type _src_type, llvm_value *_src,
                                    llvm_type _desired, llvm_value *_rec);
    llvm_instruction* create_uitofp(llvm_type _src_type, llvm_value *_src,
                                    llvm_type _desired, llvm_value *_rec);
    llvm_instruction* create_fptosi(llvm_type _src_type, llvm_value *_src,
                                    llvm_type _desired, llvm_value *_rec);
    llvm_instruction* create_sitofp(llvm_type _src_type, llvm_value *_src,
                                    llvm_type _desired, llvm_value *_rec);
    llvm_instruction* create_inttoptr(llvm_type _src_type, llvm_value *_src,
                                      llvm_type _desired, llvm_value *_rec);
    llvm_instruction* create_ptrtoint(llvm_type _src_type, llvm_value *_src,
                                      llvm_type _desired, llvm_value *_rec);
    llvm_instruction* create_bitcast(llvm_type _src_type, llvm_value *_src,
                                     llvm_type _desired, llvm_value *_rec);

    /// @fn memory accessing operations
    llvm_instruction* create_alloca(llvm_type *_type, quint32 _array_size,
                                    llvm_value *_receiver);
    llvm_instruction* create_load(llvm_type *_type, llvm_value *_ptr,
                                  llvm_value *_rec);
    llvm_instruction* create_store(llvm_type _type,
                                   llvm_value *_ptr, llvm_value *_val);
    llvm_instruction* create_getelementptr(llvm_type _type, llvm_value *_ptr,
                                           llvm_type _ty, llvm_value *_idx,
                                           llvm_value *_rec);

    /// @fn comparsions
    llvm_instruction* create_icmp_eq(llvm_type _type, llvm_value *_left,
                                     llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_icmp_ne(llvm_type _type, llvm_value *_left,
                                     llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_icmp_ugt(llvm_type _type, llvm_value *_left,
                                      llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_icmp_uge(llvm_type _type, llvm_value *_left,
                                      llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_icmp_ult(llvm_type _type, llvm_value *_left,
                                      llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_icmp_ule(llvm_type _type, llvm_value *_left,
                                      llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_icmp_sgt(llvm_type _type, llvm_value *_left,
                                      llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_icmp_sge(llvm_type _type, llvm_value *_left,
                                      llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_icmp_slt(llvm_type _type, llvm_value *_left,
                                      llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_icmp_sle(llvm_type _type, llvm_value *_left,
                                      llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_fcmp_oeq(llvm_type _type, llvm_value *_left,
                                      llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_fcmp_one(llvm_type _type, llvm_value *_left,
                                      llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_fcmp_ogt(llvm_type _type, llvm_value *_left,
                                      llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_fcmp_oge(llvm_type _type, llvm_value *_left,
                                      llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_fcmp_olt(llvm_type _type, llvm_value *_left,
                                      llvm_value *_right, llvm_value *_rec);
    llvm_instruction* create_fcmp_ole(llvm_type _type, llvm_value *_left,
                                      llvm_value *_right, llvm_value *_rec);

    /// @fn functions dealing with values
    llvm_value *create_string_constant(saber_string_view _str);
    llvm_value *create_signed_constant(qint64 _i);
    llvm_value *create_unsigned_constant(quint64 _u);
    llvm_value *create_floating_constant(qreal _r);
    llvm_value *create_bool_constant(bool _b);

    llvm_value *create_temporary_var();
    llvm_value *create_named_var(saber_string_view _name);

private:
    detail::llvm_ir_builder_impl* impl;
};

} // namespace faker

#endif // LLVM_IR_BUILDER_HPP
