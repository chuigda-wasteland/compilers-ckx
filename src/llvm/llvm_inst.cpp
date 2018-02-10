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

#include "llvm/llvm_inst_detail.hpp"
#include "saber/c8assert.hpp"

namespace faker
{

llvm_instruction::~llvm_instruction() {}

llvm_func_decl::llvm_func_decl(
        llvm_type _return_type,
        saber_string_view _name,
        saber::vector<llvm_type> &&_param_type_list,
        saber::vector<saber_string_view> &&_param_name_list,
        llvm_func_attrs _attrs) :
    return_type(_return_type),
    name(_name),
    param_type_list(saber::move(_param_type_list)),
    param_name_list(saber::move(_param_name_list)),
    attrs(_attrs) {}

void llvm_func_attrs::pretty_print(we::we_file_writer &_writer)
{
    if (nounwind) _writer.write(" nounwind ");
    switch (inlining)
    {
    case it_default:       _writer.write(" ");              break;
    case it_defaultinline: _writer.write(" inline ");       break;
    case it_noinline:      _writer.write(" noinline ");     break;
    case it_always:        _writer.write(" alwaysinline "); break;
    }
}

void llvm_func_decl::pretty_print(we::we_file_writer &_writer)
{
    _writer.write("declare ");
    _writer.write(return_type);
    _writer.write(" @");
    _writer.write(name);
    _writer.write(" (");
    for (size_t i = 0; i < param_type_list.size(); i++)
    {
        _writer.write(param_type_list[i]);
        _writer.write(" %");
        _writer.write(param_name_list[i]);
        if (i != param_type_list.size()-1) _writer.write(", ");
    }
    _writer.write(") ");
    attrs.pretty_print(_writer);
    _writer.write(";\n");
}

llvm_func_def::llvm_func_def(
        llvm_type _return_type, saber_string_view _name,
        saber::vector<llvm_type> &&_param_type_list,
        saber::vector<saber_string_view> &&_param_name_list,
        llvm_func_attrs _attrs) :
    return_type(_return_type),
    name(_name),
    param_type_list(saber::move(_param_type_list)),
    param_name_list(saber::move(_param_name_list)),
    attrs(_attrs) {}

llvm_func_def::~llvm_func_def()
{
    llvm_instruction* iter = get_next();
    while (iter != nullptr)
    {
        llvm_instruction* to_be_deleted = iter;
        iter = iter->get_next();
        delete to_be_deleted;
    }
}

void llvm_func_def::pretty_print(we::we_file_writer &_writer)
{
    _writer.write("define ");
    _writer.write(return_type);
    _writer.write(" @");
    _writer.write(name);
    _writer.write(" (");
    for (size_t i = 0; i < param_type_list.size(); i++)
    {
        _writer.write(param_type_list[i]);
        _writer.write(" %");
        _writer.write(param_name_list[i]);
        if (i != param_type_list.size()-1) _writer.write(", ");
    }
    _writer.write(") ");
    attrs.pretty_print(_writer);
    _writer.write("{\n");

    llvm_instruction* iter = get_next();
    while (iter != nullptr)
    {
        _writer.write_whitespace(2);
        iter->pretty_print(_writer);
        iter = iter->get_next();
    }
    _writer.write("}\n");
}

llvm_global_constant::llvm_global_constant(llvm_value *_value) :
    value(_value) {}

void llvm_global_constant::pretty_print(we::we_file_writer&)
{
    /// @todo I don't know what to do at all!
}

llvm_global_variable::llvm_global_variable(llvm_type _type,
                                           saber_string_view _name) :
    type(_type),
    name(_name) {}

void llvm_global_variable::pretty_print(we::we_file_writer&)
{
    /// @todo I don't know what to do at all!
}

llvm_new_type::llvm_new_type(saber_string_view _name,
                             saber::vector<llvm_type> &&_fields) :
    name(_name),
    fields(saber::move(_fields)) {}

void llvm_new_type::pretty_print(we::we_file_writer &_writer)
{
    _writer.write("%");
    _writer.write(name);
    _writer.write(" = type {");
    for (size_t i = 0; i < fields.size(); i++)
    {
        _writer.write(fields[i]);
        if (i != fields.size()-1)
            _writer.write(", ");
    }
    _writer.write("}\n");
}

saber_string_view llvm_label::get_label_name() const
{
    return name;
}

void llvm_label::pretty_print(we::we_file_writer &_writer)
{
    _writer.write(name);
    _writer.write(":\n");
}

llvm_binary_instruction::llvm_binary_instruction(llvm_value *_result,
                                                 operator_type _op,
                                                 llvm_type _yield_type,
                                                 llvm_value *_lhs,
                                                 llvm_value *_rhs) :
    result(_result),
    op(_op),
    yield_type(_yield_type),
    lhs(_lhs),
    rhs(_rhs)
{}

void llvm_binary_instruction::pretty_print(we::we_file_writer &_writer)
{
    /// @todo Lots of work! Fuck!
    _writer.write(result->to_string());
    _writer.write(" = ");
    switch (op)
    {
    case operator_type::ot_add:  _writer.write("add ");  break;
    case operator_type::ot_sub:  _writer.write("sub ");  break;
    case operator_type::ot_mul:  _writer.write("mul ");  break;
    case operator_type::ot_sdiv: _writer.write("sdiv "); break;
    case operator_type::ot_udiv: _writer.write("udiv "); break;
    case operator_type::ot_srem: _writer.write("srem "); break;
    case operator_type::ot_urem: _writer.write("urem "); break;
    case operator_type::ot_smod: _writer.write("smod "); break;
    case operator_type::ot_umod: _writer.write("umod "); break;
    case operator_type::ot_fadd: _writer.write("fadd "); break;
    case operator_type::ot_fsub: _writer.write("fsub "); break;
    case operator_type::ot_fmul: _writer.write("fmul "); break;
    case operator_type::ot_fdiv: _writer.write("fdiv "); break;
    case operator_type::ot_and:  _writer.write("and ");  break;
    case operator_type::ot_or:   _writer.write("or ");   break;
    case operator_type::ot_xor:  _writer.write("xor ");  break;
    case operator_type::ot_shl:  _writer.write("shl ");  break;
    case operator_type::ot_lshr: _writer.write("lshr "); break;
    case operator_type::ot_ashr: _writer.write("ashr "); break;
    default: C8ASSERT(false);
    }
    _writer.write(yield_type);
    _writer.write(" ");
    _writer.write(lhs->to_string());
    _writer.write(", ");
    _writer.write(rhs->to_string());
    _writer.write("\n");
}

llvm_cast_instruction::llvm_cast_instruction(llvm_value *_result,
                                             operator_type _op,
                                             llvm_type _origin_type,
                                             llvm_value *_value,
                                             llvm_type _dest_type) :
    result(_result),
    op(_op),
    origin_type(_origin_type),
    value(_value),
    dest_type(_dest_type) {}

void llvm_cast_instruction::pretty_print(we::we_file_writer &_writer)
{
    _writer.write(result->to_string());
    _writer.write(" = ");
    switch (op)
    {
    case operator_type::ot_trunc:    _writer.write("trunc ");    break;
    case operator_type::ot_zext:     _writer.write("zext ");     break;
    case operator_type::ot_sext:     _writer.write("sext ");     break;
    case operator_type::ot_fpext:    _writer.write("fpext ");    break;
    case operator_type::ot_fptoui:   _writer.write("fptoui ");   break;
    case operator_type::ot_uitofp:   _writer.write("uitofp ");   break;
    case operator_type::ot_fptosi:   _writer.write("fptosi ");   break;
    case operator_type::ot_sitofp:   _writer.write("sitofp ");   break;
    case operator_type::ot_inttoptr: _writer.write("inttoptr "); break;
    case operator_type::ot_ptrtoint: _writer.write("ptrtoint "); break;
    case operator_type::ot_bitcast:  _writer.write("bitcast ");  break;
    default: C8ASSERT(false);
    }
    _writer.write(origin_type);
    _writer.write(" ");
    _writer.write(value->to_string());
    _writer.write(" to ");
    _writer.write(dest_type);
    _writer.write("\n");
}

llvm_cmp_instruction::llvm_cmp_instruction(llvm_value *_result,
                                           comparsion_type _op,
                                           llvm_type _compared_type,
                                           llvm_value *_val1,
                                           llvm_value *_val2) :
    result(_result),
    op(_op),
    compared_type(_compared_type),
    val1(_val1), val2(_val2) {}

void llvm_cmp_instruction::pretty_print(we::we_file_writer &_writer)
{
    _writer.write(result->to_string());
    _writer.write(" = ");

    qchar& comp_type_bitmask = reinterpret_cast<qchar&>(op);
    if (comp_type_bitmask & 0x20)
    {
        _writer.write("fcmp o");
    }
    else
    {
        _writer.write("icmp ");
        if (comp_type_bitmask & 0x10)
            _writer.write("u");
        else if (comp_type_bitmask & 0x04)
            _writer.write("s");
    }

    switch (comp_type_bitmask & 0x04)
    {
    case 0: _writer.write("ne "); break;
    case 1: _writer.write("eq "); break;
    case 2: _writer.write("gt "); break;
    case 3: _writer.write("ge "); break;
    case 4: _writer.write("lt "); break;
    case 5: _writer.write("le "); break;
    default: C8ASSERT(false);
    }

    _writer.write(compared_type);
    _writer.write(" ");
    _writer.write(val1->to_string());
    _writer.write(", ");
    _writer.write(val2->to_string());
    _writer.write("\n");
}

llvm_call_instruction::llvm_call_instruction(
        llvm_value *_result,
        llvm_type _yield_type,
        saber_string_view _func_name,
        saber::vector<llvm_type> &&_types,
        saber::vector<llvm_value *> &&_args) :
    result(_result),
    yield_type(_yield_type),
    func_name(_func_name),
    types(saber::move(_types)),
    args(saber::move(_args)) {}

void llvm_call_instruction::pretty_print(we::we_file_writer &_writer)
{
    if (result != nullptr)
    {
        _writer.write(result->to_string());
        _writer.write(" = ");
    }

    _writer.write("call ");
    _writer.write(yield_type);

    _writer.write(" @");
    _writer.write(func_name);

    _writer.write(" (");
    for (size_t i = 0; i < args.size(); i++)
    {
        _writer.write(types[i]);
        _writer.write(" ");
        _writer.write(args[i]->to_string());
        if (i != args.size()-1)
            _writer.write(", ");
    }
    _writer.write(")\n");
}

llvm_ret_instruction::llvm_ret_instruction(llvm_type _type,
                                           llvm_value *_result) :
    type(_type),
    result(_result) {}

void llvm_ret_instruction::pretty_print(we::we_file_writer &_writer)
{
    _writer.write("ret ");
    _writer.write(type);
    if (result != nullptr)
    {
        _writer.write(" ");
        _writer.write(result->to_string());
    }
    _writer.write("\n");
}

llvm_br_instruction::llvm_br_instruction(llvm_label *_dest) :
    dest(_dest) {}

void llvm_br_instruction::pretty_print(we::we_file_writer &_writer)
{
    _writer.write("br label %");
    _writer.write(dest->get_label_name());
    _writer.write("\n");
}

llvm_condbr_instruction::llvm_condbr_instruction(llvm_value *_condition,
                                                 llvm_label *_true_label,
                                                 llvm_label *_false_label) :
    condition(_condition),
    true_label(_true_label),
    false_label(_false_label)
{}

void llvm_condbr_instruction::pretty_print(we::we_file_writer &_writer)
{
    _writer.write("br i1 ");
    _writer.write(condition->to_string());
    _writer.write(", label %");
    _writer.write(true_label->get_label_name());
    _writer.write(", label %");
    _writer.write(false_label->get_label_name());
    _writer.write("\n");
}

llvm_phi_instruction::llvm_phi_instruction(llvm_value *_result,
                                           llvm_type _yield_type,
                                           llvm_label *_label1,
                                           llvm_value *_val1,
                                           llvm_label *_label2,
                                           llvm_value *_val2) :
    result(_result),
    yield_type(_yield_type),
    label1(_label1), label2(_label2),
    val1(_val1), val2(_val2) {}

void llvm_phi_instruction::pretty_print(we::we_file_writer &_writer)
{
    _writer.write(result->to_string());
    _writer.write(" = phi ");
    _writer.write(yield_type);
    _writer.write(" [");
    _writer.write(val1->to_string());
    _writer.write(", %");
    _writer.write(label1->get_label_name());
    _writer.write("], [");
    _writer.write(val2->to_string());
    _writer.write(", %");
    _writer.write(label2->get_label_name());
    _writer.write("]\n");
}

llvm_alloca_instruction::llvm_alloca_instruction(llvm_value *_result,
                                                 llvm_type _yield_type,
                                                 quint32 _num_elems) :
    result(_result),
    yield_type(_yield_type),
    num_elems(_num_elems) {}

void llvm_alloca_instruction::pretty_print(we::we_file_writer &_writer)
{
    _writer.write(result->to_string());
    _writer.write(" = alloca ");
    _writer.write(yield_type);
    if (num_elems != 1)
    {
        _writer.write(", i32 ");
        _writer.write(quint64(num_elems));
    }
    _writer.write("\n");
}

llvm_load_instruction::llvm_load_instruction(llvm_value *_result,
                                             llvm_type _yield_type,
                                             llvm_value *_ptr) :
    result(_result),
    yield_type(_yield_type),
    ptr(_ptr) {}

void llvm_load_instruction::pretty_print(we::we_file_writer &_writer)
{
    _writer.write(result->to_string());
    _writer.write(" = load ");
    _writer.write(yield_type);
    _writer.write(", ");
    _writer.write(yield_type);
    _writer.write("* ");
    _writer.write(ptr->to_string());
    _writer.write("\n");
}

llvm_store_instruction::llvm_store_instruction(llvm_type _src_type,
                                               llvm_value *_src,
                                               llvm_value *_result) :
    src_type(_src_type),
    src(_src),
    result(_result) {}

void llvm_store_instruction::pretty_print(we::we_file_writer &_writer)
{
    _writer.write("store ");
    _writer.write(src_type);
    _writer.write(" ");
    _writer.write(src->to_string());
    _writer.write(", ");
    _writer.write(src_type);
    _writer.write("* ");
    _writer.write(result->to_string());
    _writer.write("\n");
}

llvm_extractvalue_instruction::llvm_extractvalue_instruction(
        llvm_value *_result,
        llvm_type _aggregate,
        llvm_value *_val,
        llvm_value *_idx) :
    result(_result),
    aggregate(_aggregate),
    val(_val),
    idx(_idx) {}

void llvm_extractvalue_instruction::pretty_print(we::we_file_writer &_writer)
{
    _writer.write(result->to_string());
    _writer.write(" = extractvalue ");
    _writer.write(aggregate);
    _writer.write(" ");
    _writer.write(val->to_string());
    _writer.write(", ");
    _writer.write(idx->to_string());
    _writer.write("\n");
}

llvm_getelementptr_instruction::llvm_getelementptr_instruction(
        llvm_value *_result,
        llvm_type _base_type,
        llvm_value *_ptr,
        llvm_type _ty,
        llvm_value *_idx) :
    result(_result),
    base_type(_base_type),
    ptr(_ptr),
    ty(_ty),
    idx(_idx) {}

void llvm_getelementptr_instruction::pretty_print(we::we_file_writer &_writer)
{
    _writer.write(result->to_string());
    _writer.write(" = getelementptr ");
    _writer.write(base_type);
    _writer.write(" ");
    _writer.write(base_type);
    _writer.write("* ");
    _writer.write(ptr->to_string());
    _writer.write(", ");
    _writer.write(ty);
    _writer.write(", ");
    _writer.write(idx->to_string());
    _writer.write("\n");
}

} // namespace faker
