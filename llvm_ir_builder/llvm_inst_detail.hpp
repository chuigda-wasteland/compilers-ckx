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

#ifndef LLVM_INST_DETAIL_HPP
#define LLVM_INST_DETAIL_HPP

#include "llvm_inst.hpp"
#include "llvm_type.hpp"
#include "llvm_value.hpp"
#include "vector.hpp"

namespace faker
{

/// @brief declare <return-type> @<name> (<param_type_list x param_name_list>)
///                <attributes> ;
class llvm_func_decl final implements llvm_instruction
{
public:
    llvm_func_decl(llvm_type _return_type,
                   saber_string_view _name,
                   saber::vector<llvm_type>&& _param_type_list,
                   saber::vector<saber_string_view>&& _param_name_list,
                   llvm_func_attrs _attrs);
    ~llvm_func_decl() override final = default;

private:
    llvm_type return_type;
    saber_string_view name;
    saber::vector<llvm_type> param_type_list;
    saber::vector<saber_string_view> param_name_list;
    llvm_func_attrs attrs;
};


/// @brief define <return-type> @<name> (<param_type_list x param_name_list>)
///                             <attributes> { <...> }
class llvm_func_def final implements llvm_instruction
{
public:
    llvm_func_def(llvm_type _return_type,
                  saber_string_view _name,
                  saber::vector<llvm_type>&& _param_type_list,
                  saber::vector<saber_string_view>&& _param_name_list,
                  llvm_func_attrs _attrs);
    ~llvm_func_def() override final = default;

private:
    llvm_type return_type;
    saber_string_view name;
    saber::vector<llvm_type> param_type_list;
    saber::vector<saber_string_view> param_name_list;
    llvm_func_attrs attrs;
};


/// @brief I don't know what to do!
class llvm_global_constant final implements llvm_instruction
{
public:
    explicit llvm_global_constant(llvm_value* _value);
    ~llvm_global_constant() override final = default;

private:
    llvm_value *value;
};


/// @brief I don't know what to do, too!
class llvm_global_variable final implements llvm_instruction
{
public:
    llvm_global_variable(llvm_type _type, saber_string_view _name);
    ~llvm_global_variable() override final = default;

private:
    llvm_type type;
    saber_string_view name;
};


/// @brief %<name> = type { <fields> }
class llvm_new_type final implements llvm_instruction
{
public:
    llvm_new_type(saber_string_view _name, saber::vector<llvm_type>&& _fields);
    ~llvm_new_type() override final = default;

private:
    saber_string_view name;
    saber::vector<llvm_type> fields;
};


/// @brief <name>:
class llvm_label final implements llvm_instruction
{
public:
    explicit llvm_label() = default;
    ~llvm_label() override final = default;

    void set_label_name(saber_string_view _name);
    saber_string_view get_label_name() const;

private:
    saber_string_view name;

    DEBUG_EXEC( bool name_set; )
};


/// @brief <result> = <op> <yield_type> <lhs>, <rhs>
class llvm_binary_instruction final implements llvm_instruction
{
public:
    enum class operator_type : qchar
    {
        ot_add, ot_sub, ot_mul, ot_sdiv, ot_udiv,
        ot_srem, ot_urem, ot_smod, ot_umod,
        ot_fadd, ot_fsub, ot_fmul, ot_fdiv,
        ot_and, ot_or, ot_xor, ot_shl, ot_lshr, ot_ashr
    };

    llvm_binary_instruction(llvm_value *_result,
                            operator_type _op,
                            llvm_type _yield_type,
                            llvm_value *_lhs,
                            llvm_value *_rhs);
    ~llvm_binary_instruction() override final = default;

private:
    operator_type op;
    llvm_type yield_type;
    llvm_value *lhs, *rhs;
    llvm_value *result;
};


/// @brief <result> = <op> <origin_type> <value> to <dest_type>
class llvm_cast_instruction final implements llvm_instruction
{
public:
    enum class operator_type : qchar
    {
        ot_trunc, ot_zext, ot_sext,
        ot_fptoui, ot_uitofp, ot_fptosi, ot_sitofp,
        ot_inttoptr, ot_ptrtoint,
        ot_bitcast
    };

    llvm_cast_instruction(llvm_value *_result,
                          operator_type _op,
                          llvm_type _origin_type,
                          llvm_value *_value,
                          llvm_type _dest_type);
    ~llvm_cast_instruction() override final = default;

private:
    llvm_value *result;
    operator_type op;
    llvm_type origin_type;
    llvm_value *value;
    llvm_type dest_type;
};


/// @brief <result> = <cmp> <type> <val1> <val2>
class llvm_cmp_instruction final implements llvm_instruction
{
public:
    /// @authors Thanks to LLVM! Clever this is!
    enum class comparsion_type : qchar
    {
        /// OPERCODE        =           U  L  G  E
        ot_icmp_sne  =  0,     ///      0  0  0  0
        ot_icmp_seq  =  1,     ///      0  0  0  1
        ot_icmp_sgt  =  2,     ///      0  0  1  0
        ot_icmp_sge  =  3,     ///      0  0  1  1
        ot_icmp_slt  =  4,     ///      0  1  0  0
        ot_icmp_sle  =  5,     ///      0  1  0  1
        __UUSED0  =  6,        ///      0  X  X  0
        __UUSED1  =  7,        ///      0  X  X  X
        ot_icmp_une  =  8,     ///      1  0  0  0
        ot_icmp_ueq  =  9,     ///      1  0  0  1
        ot_icmp_ugt  = 10,     ///      1  0  1  0
        ot_icmp_uge  = 11,     ///      1  0  1  1
        ot_icmp_ult  = 12,     ///      1  1  0  0
        ot_icmp_ule  = 13,     ///      1  1  0  1
        __UUSED2  = 14,        ///      1  X  X  0
        __UUSED3  = 15,        ///      1  X  X  X

        /// OPERCODE        =        F  _  L  G  E
        ot_fcmp_one   = 32,    ///   1  0  0  0  0
        ot_fcmp_oeq   = 33,    ///   1  0  0  0  1
        ot_fcmp_ogt   = 34,    ///   1  0  0  1  0
        ot_fcmp_oge   = 35,    ///   1  0  0  1  1
        ot_fcmp_olt   = 36,    ///   1  0  1  0  0
        ot_fcmp_ole   = 37,    ///   1  0  1  0  1
    };

    llvm_cmp_instruction(llvm_value *_result, comparsion_type _op,
                         llvm_type _compared_type,
                         llvm_value *_val1, llvm_value *_val2);
    ~llvm_cmp_instruction() override final = default;

private:
    llvm_value *result;
    comparsion_type op;
    llvm_type compared_type;
    llvm_value *val1, *val2;
};


/// @brief call <yield_type> @<func_name> (<args>)
class llvm_call_instruction final implements llvm_instruction
{
public:
    llvm_call_instruction(saber_string_view _func_name,
                          llvm_type _yield_type,
                          saber::vector<llvm_value*>&& _args,
                          llvm_value *_result = nullptr);
    ~llvm_call_instruction() override final = default;

private:
    saber_string_view func_name;
    llvm_type yield_type;
    saber::vector<llvm_value*> args;
    llvm_value *result;
};


/// @brief
/// ret void
/// ret <type> <value>
class llvm_ret_instruction final implements llvm_instruction
{
public:
    llvm_ret_instruction(llvm_type _type, llvm_value* _value);
    ~llvm_ret_instruction() override final = default;

private:
    llvm_type yield_type;
    llvm_value *result;
};


/// @brief br label %<dest>
class llvm_br_instruction final implements llvm_instruction
{
public:
    explicit llvm_br_instruction(llvm_label* _dest);
    ~llvm_br_instruction() override final = default;

private:
    llvm_label *dest;
};


/// @brief br i1 <condition> label %<true_label>, label %<false_label>
class llvm_condbr_instruction final implements llvm_instruction
{
public:
    llvm_condbr_instruction(llvm_value *_condition,
                            llvm_label *_true_label,
                            llvm_label *_false_label);
    ~llvm_condbr_instruction() override final = default;

private:
    llvm_value *condition;
    llvm_label *true_label, *false_label;
};

/// @brief <result> = phi <yield_type> <label1> <val1> <label2> <val2>
class llvm_phi_instruction final implements llvm_instruction
{
public:
    llvm_phi_instruction(llvm_value *_result,
                         llvm_type _yield_type,
                         llvm_label *_label1, llvm_value* _val1,
                         llvm_label *_label2, llvm_value* _val2);
    ~llvm_phi_instruction() override final = default;

private:
    llvm_value *result;
    llvm_type yield_type;
    llvm_label *label1, &label2;
    llvm_value *val1, *val2;
};

/// @brief <result> = alloca <yield_type> <num_elems>
class llvm_alloca_instruction final implements llvm_instruction
{
public:
    llvm_alloca_instruction(llvm_value *_result,
                            llvm_type _yield_type,
                            quint32 _num_elems);
    ~llvm_alloca_instruction() override final = default;

private:
    llvm_value *result;
    llvm_type yield_type;
    quint32 num_elems;
};


/// @brief <result> = load <yield_type>, <yield_type>* <ptr>
class llvm_load_instruction final implements llvm_instruction
{
public:
    llvm_load_instruction(llvm_value *_result,
                          llvm_type _yield_type,
                          llvm_value *_ptr);
    ~llvm_load_instruction() override final = default;

private:
    llvm_value *result;
    llvm_type yield_type;
    llvm_value *ptr;
};


/// @brief store <src_type> <src>, <src_type>* <result>
class llvm_store_instruction final implements llvm_instruction
{
public:
    llvm_store_instruction(llvm_type _src_type,
                           llvm_value *_src,
                           llvm_value *_result);
    ~llvm_store_instruction() override final = default;

private:
    llvm_type src_type;
    llvm_value *src;
    llvm_value *result;
};


/// @brief
/// <result> = getelementptr <yield_type> <yield_type>* <ptr> <ty> <idx>
/// @note we are not about to support multiple "<ty> <idx>".
class llvm_getelementptr_instruction final implements llvm_instruction
{
public:
    llvm_getelementptr_instruction(llvm_value *_result,
                                   llvm_type _yield_type,
                                   llvm_value* _ptr,
                                   llvm_type _ty,
                                   llvm_value* _idx);
    ~llvm_getelementptr_instruction() override final = default;

private:
    llvm_value *result;
    llvm_type yield_type;
    llvm_value *ptr;
    llvm_type ty;
    llvm_value *idx;
};

} // namespace faker

#endif // LLVM_INST_DETAIL_HPP
